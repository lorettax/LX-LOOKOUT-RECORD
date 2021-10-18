#include "eventloop.hh"

#include "util.hh"

#include <cerrno>
#include <stdexcept>
#include <system_error>
#include <utility>
#include <vector>

using namespace std;

unsigned int EventLoop::Rule::service_count() const {
    return direction == Direction::In ? fd.read_count() : fd.write_count();
}

//! \param[in] fd is the FileDescriptor to be polled
//! \param[in] direction indicates whether to poll for reading (Direction::In) or writing (Direction::Out)
//! \param[in] callback is called when `fd` is ready.
//! \param[in] interest is called by EventLoop::wait_next_event. If it returns `true`, `fd` will
//!                     be polled, otherwise `fd` will be ignored only for this execution of `wait_next_event.
//! \param[in] cancel is called when the rule is cancelled (e.g. on hangup, EOF, or closure).
void EventLoop::add_rule(const FileDescriptor &fd,
                         const Direction direction,
                         const CallbackT &callback,
                         const InterestT &interest,
                         const CallbackT &cancel) {
    _rules.push_back({fd.duplicate(), direction, callback, interest, cancel});
}

//! \param[in] timeout_ms is the timeout value passed to [poll(2)](\ref man2::poll); `wait_next_event`
//!                       returns Result::Timeout if no fd is ready after the timeout expires.
//! \returns Eventloop::Result indicating success, timeout, or no more Rule objects to poll.
EventLoop::Result EventLoop::wait_next_event(const int timeout_ms) {
    vector<pollfd> pollfds{};
    pollfds.reserve(_rules.size());
    bool something_to_poll = false;

    // set up the pollfd for each rule
    for (auto it = _rules.cbegin(); it != _rules.cend();) {  // NOTE: it gets erased or incremented in loop body
        const auto &this_rule = *it;
        if (this_rule.direction == Direction::In && this_rule.fd.eof()) {
            // no more reading on this rule, it's reached eof
            this_rule.cancel();
            it = _rules.erase(it);
            continue;
        }

        if (this_rule.fd.closed()) {
            this_rule.cancel();
            it = _rules.erase(it);
            continue;
        }

        if (this_rule.interest()) {
            pollfds.push_back({this_rule.fd.fd_num(), static_cast<short>(this_rule.direction), 0});
            something_to_poll = true;
        } else {
            pollfds.push_back({this_rule.fd.fd_num(), 0, 0});  // placeholder --- we still want errors
        }
        ++it;
    }

    // quit if there is nothing left to poll
    if (not something_to_poll) {
        return Result::Exit;
    }

    // call poll -- wait until one of the fds satisfies one of the rules (writeable/readable)
    try {
        if (0 == SystemCall("poll", ::poll(pollfds.data(), pollfds.size(), timeout_ms))) {
            return Result::Timeout;
        }
    } catch (unix_error const &e) {
        if (e.code().value() == EINTR) {
            return Result::Exit;
        }
    }

    // go through the poll results

    for (auto [it, idx] = make_pair(_rules.begin(), size_t(0)); it != _rules.end(); ++idx) {
        const auto &this_pollfd = pollfds[idx];

        const auto poll_error = static_cast<bool>(this_pollfd.revents & (POLLERR | POLLNVAL));
        if (poll_error) {
            throw runtime_error("EventLoop: error on polled file descriptor");
        }

        const auto &this_rule = *it;
        const auto poll_ready = static_cast<bool>(this_pollfd.revents & this_pollfd.events);
        const auto poll_hup = static_cast<bool>(this_pollfd.revents & POLLHUP);
        if (poll_hup && this_pollfd.events && !poll_ready) {
            // if we asked for the status, and the _only_ condition was a hangup, this FD is defunct:
            //   - if it was POLLIN and nothing is readable, no more will ever be readable
            //   - if it was POLLOUT, it will not be writable again
            this_rule.cancel();
            it = _rules.erase(it);
            continue;
        }

        if (poll_ready) {
            // we only want to call callback if revents includes the event we asked for
            const auto count_before = this_rule.service_count();
            this_rule.callback();

            // only check for busy wait if we're not canceling or exiting
            if (count_before == this_rule.service_count() and this_rule.interest()) {
                throw runtime_error(
                    "EventLoop: busy wait detected: callback did not read/write fd and is still interested");
            }
        }

        ++it;  // if we got here, it means we didn't call _rules.erase()
    }

    return Result::Success;
}
