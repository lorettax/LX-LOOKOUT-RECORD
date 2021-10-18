#ifndef LXLOOKOUTRECORD_LIBLOOKOUT_TCP_LXLOOKOUTRECORD_SOCKET_HH
#define LXLOOKOUTRECORD_LIBLOOKOUT_TCP_LXLOOKOUTRECORD_SOCKET_HH

#include "byte_stream.hh"
#include "eventloop.hh"
#include "fd_adapter.hh"
#include "file_descriptor.hh"
#include "tcp_config.hh"
#include "tcp_connection.hh"
#include "tunfd_adapter.hh"

#include <atomic>
#include <cstdint>
#include <optional>
#include <thread>
#include <vector>

//! Multithreaded wrapper around TCPConnection that approximates the Unix sockets API
template <typename AdaptT>
class TCPLookOutRecordSocket : public LocalStreamSocket {
  private:
    //! Stream socket for reads and writes between owner and TCP thread
    LocalStreamSocket _thread_data;

    //! Adapter to underlying datagram socket (e.g., UDP or IP)
    AdaptT _datagram_adapter;

    //! Set up the TCPConnection and the event loop
    void _initialize_TCP(const TCPConfig &config);

    //! TCP state machine
    std::optional<TCPConnection> _tcp{};

    //! eventloop that handles all the events (new inbound datagram, new outbound bytes, new inbound bytes)
    EventLoop _eventloop{};

    //! Process events while specified condition is true
    void _tcp_loop(const std::function<bool()> &condition);

    //! Main loop of TCPConnection thread
    void _tcp_main();

    //! Handle to the TCPConnection thread; owner thread calls join() in the destructor
    std::thread _tcp_thread{};

    //! Construct LocalStreamSocket fds from socket pair, initialize eventloop
    TCPLookOutRecordSocket(std::pair<FileDescriptor, FileDescriptor> data_socket_pair, FileDescriptor &&dgramfd);

    std::atomic_bool _abort{false};  //!< Flag used by the owner to force the TCPConnection thread to shut down

    bool _inbound_shutdown{false};  //!< Has TCPLookOutRecordSocket shut down the incoming data to the owner?

    bool _outbound_shutdown{false};  //!< Has the owner shut down the outbound data to the TCP connection?

    bool _fully_acked{false};  //!< Has the outbound data been fully acknowledged by the peer?

  public:
    //! Construct from the FileDescriptor that the TCPConnection thread will use to read and write datagrams
    explicit TCPLookOutRecordSocket(FileDescriptor &&dgramfd);

    //! Close socket, and wait for TCPConnection to finish
    //! \note Calling this function is only advisable if the socket has reached EOF,
    //! or else may wait foreever for remote peer to close the TCP connection.
    void wait_until_closed();

    //! Connect using the specified configurations; blocks until connect succeeds or fails
    void connect(const TCPConfig &c_tcp, const FdAdapterConfig &c_ad);

    //! Listen and accept using the specified configurations; blocks until accept succeeds or fails
    void listen_and_accept(const TCPConfig &c_tcp, const FdAdapterConfig &c_ad);

    //! When a connected socket is destructed, it will send a RST
    ~TCPLookOutRecordSocket();

    //! \name
    //! This object cannot be safely moved or copied, since it is in use by two threads simultaneously

    //!@{
    TCPLookOutRecordSocket(const TCPLookOutRecordSocket &) = delete;
    TCPLookOutRecordSocket(TCPLookOutRecordSocket &&) = delete;
    TCPLookOutRecordSocket &operator=(const TCPLookOutRecordSocket &) = delete;
    TCPLookOutRecordSocket &operator=(TCPLookOutRecordSocket &&) = delete;
    //!@}

    //! \name
    //! Some methods of the parent Socket wouldn't work as expected on the TCP socket, so delete them

    //!@{
    void bind(const Address &address) = delete;
    Address local_address() const = delete;
    Address peer_address() const = delete;
    void set_reuseaddr() = delete;
    //!@}
};

using TCPOverUDPLookOutRecordSocket = TCPLookOutRecordSocket<TCPOverUDPSocketAdapter>;
using TCPOverIPv4LookOutRecordSocket = TCPLookOutRecordSocket<TCPOverIPv4OverTunFdAdapter>;

using LossyTCPOverUDPLookOutRecordSocket = TCPLookOutRecordSocket<LossyTCPOverUDPSocketAdapter>;
using LossyTCPOverIPv4LookOutRecordSocket = TCPLookOutRecordSocket<LossyTCPOverIPv4OverTunFdAdapter>;

//! \class 
//! This class involves the simultaneous operation of two threads.

//! Helper class that makes a TCPOverIPv4LookOutRecordSocket behave more like a (kernel) TCPSocket
class LXTCPSocket : public TCPOverIPv4LookOutRecordSocket {
  public:
    LXTCPSocket();
    void connect(const Address &address);
};

#endif 
