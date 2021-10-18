#include "tcp_connection.hh"

#include <iostream>

// implementation of a TCP connection


template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

size_t TCPConnection::remaining_outbound_capacity() const 
{ return _sender.stream_in().remaining_capacity(); }

size_t TCPConnection::bytes_in_flight() const 
{ return _sender.bytes_in_flight(); }

size_t TCPConnection::unassembled_bytes() const 
{ return _receiver.unassembled_bytes(); }

size_t TCPConnection::time_since_last_segment_received() const 
{ return _time_since_last_segment_received; }

void TCPConnection::segment_received(const TCPSegment &seg) 
{
    if (!_active)
    {
        return;
    }
    _time_since_last_segment_received = 0;

    // data segments with acceptable ACKS should be ignored in SYN_SENT
    if (in_syn_sent() && seg.header().ack && seg.payload().size() > 0)
    {
        return;
    }
    bool send_empty = false;
    if (_sender.next_seqno_absolute() > 0 && seg.header().ack)
    {
        // unacceptable ACKs should produced a segment that existed
        if (!_sender.ack_received(seg.header().ackno, seg.header().win))
        {
            send_empty = true;
        }
    }

    bool  recv_flag = _receiver.segment_received(seg);
    if (!recv_flag)
    {
        send_empty = true;
    }
    
    if (seg.header().syn && _sender.next_seqno_absolute() == 0)
    {
        connect();
        return;
    }

    if (seg.header().rst)
    {
        // RST segments without ACKs should be ignored in SYN_SENT
        if (in_syn_sent() && !seg.header().ack)
        {
            return;
        }
        unclean_shutdown(false);
        return;
    }
    
    if (seg.length_in_sequence_space() > 0)
    {
        send_empty = true;
    }
    
    if (send_empty)
    {
        if (_receiver.ackno().has_value() && _sender.segments_out().empty())
        {
            _sender.send_empty_segment();
        }
    }
    push_segments_out();
    
}

bool TCPConnection::active() const 
{
     return _active; 
}

size_t TCPConnection::write(const string &data) {
    size_t ret = _sender.stream_in().write(data);
    push_segments_out();
    return ret;
}

//! \param[in] ms_since_last_tick number of milliseconds since the last call to this method
void TCPConnection::tick(const size_t ms_since_last_tick) 
{
     if(!_active)
     {
         return;
     }
     _time_since_last_segment_received += ms_since_last_tick;
     _sender.tick(ms_since_last_tick);
     if (_sender.consecutive_retransmissions() > TCPConfig::MAX_RETX_ATTEMPTS)
     {
         unclean_shutdown(true);
     }
     push_segments_out();
}

void TCPConnection::end_input_stream() 
{
    _sender.stream_in().end_input();
    push_segments_out();
}

void TCPConnection::connect() {
    // when connect, must active send a SYN
    push_segments_out(true);
}

TCPConnection::~TCPConnection() {
    try {
        if (active()) {
            cerr << "Warning: Unclean shutdown of TCPConnection\n";
            
            unclean_shutdown(true);
        }
    } catch (const exception &e) {
        std::cerr << "Exception destructing TCP FSM: " << e.what() << std::endl;
    }
}

bool TCPConnection::push_segments_out(bool send_syn)
{
    // default not send syn before recv a SYN
    _sender.fill_window(send_syn || in_syn_recv());
    TCPSegment seg;
    while (!_sender.segments_out().empty())
    {
        seg = _sender.segments_out().front();
        _sender.segments_out().pop();
        if (_receiver.ackno().has_value())
        {
            seg.header().ack = true;
            seg.header().ackno = _receiver.ackno().value();
            seg.header().win = _receiver.window_size();
        }
        if (_need_send_rst)
        {
            _need_send_rst = false;
            seg.header().rst = true;
        }
        _segments_out.push(seg);
    }
    clean_shutdown();
    return true;   
}

void TCPConnection::unclean_shutdown(bool send_rst)
{
    _receiver.stream_out().set_error();
    _sender.stream_in().set_error();
    _active = false;
    if (send_rst)
    {
        _need_send_rst = true;
        if (_sender.segments_out().empty())
        {
            _sender.send_empty_segment();
        }
        push_segments_out();
    }
}

bool TCPConnection::clean_shutdown()
{
    if (_receiver.stream_out().input_ended() && (_sender.stream_in().eof()))
    {
        _linger_after_streams_finish = false;
    }
    if (_sender.stream_in().eof() && _sender.bytes_in_flight()==0 && _receiver.stream_out().input_ended())
    {
        if (!_linger_after_streams_finish || time_since_last_segment_received() >= 10 * _cfg.rt_timeout)
        {
            _active = false;
        }
    }
    return !_active; 
}

bool TCPConnection::in_listen()
{
    return !_receiver.ackno().has_value() && _sender.next_seqno_absolute() ==0;
}

bool TCPConnection::in_syn_recv()
{
    return _receiver.ackno().has_value() && !_receiver.stream_out().input_ended();
}

bool TCPConnection::in_syn_sent()
{
    return _sender.next_seqno_absolute() > 0 && _sender.bytes_in_flight() == _sender.next_seqno_absolute();
}
