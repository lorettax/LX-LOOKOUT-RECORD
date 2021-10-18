#ifndef LXLOOKOUTRECORD_LIBLOOKOUT_TCP_RECEIVER_HH
#define LXLOOKOUTRECORD_LIBLOOKOUT_TCP_RECEIVER_HH

#include "byte_stream.hh"
#include "stream_reassembler.hh"
#include "tcp_segment.hh"
#include "wrapping_integers.hh"

#include <optional>

//! \brief The "receiver" part of a TCP implementation.

//! Receives and reassembles segments into a ByteStream, and computes
//! the acknowledgment number and window size to advertise back to the
//! remote TCPSender.
class TCPReceiver {
    //! Our data structure for re-assembling bytes.
    StreamReassembler _reassembler;

    bool _syn_flag = false;
    bool _fin_flag = false;
    size_t _base = 0;
    size_t _isn = 0;

    //! The maximum number of bytes we'll store.
    size_t _capacity;

  public:
    //! \brief Construct a TCP receiver
    //!
    //! \param capacity the maximum number of bytes that the receiver will
    //!                 store in its buffers at any give time.
    TCPReceiver(const size_t capacity) : _reassembler(capacity), _capacity(capacity) {}

    std::optional<WrappingInt32> ackno() const;

    size_t window_size() const;
    //!@}

    //! \brief number of bytes stored but not yet reassembled
    size_t unassembled_bytes() const { return _reassembler.unassembled_bytes(); }

    //! \brief handle an inbound segment
    //! \returns `true` if any part of the segment was inside the window
    bool segment_received(const TCPSegment &seg);

    //! \name "Output" interface for the reader
    //!@{
    ByteStream &stream_out() { return _reassembler.stream_out(); }
    const ByteStream &stream_out() const { return _reassembler.stream_out(); }
    //!@}
};

#endif 
