#include "tcp_receiver.hh"


#include <optional>
// implementation of a TCP receiver


template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

bool TCPReceiver::segment_received(const TCPSegment &seg) {
    bool ret = false;
    static size_t abs_seqno = 0;
    size_t length;
    if(seg.header().syn) {
        if (_syn_flag)
        {
            return false;
        }
        _syn_flag = true;
        ret = true;
        _isn = seg.header().seqno.raw_value();
        abs_seqno = 1;
        _base = 1;
        length = seg.length_in_sequence_space() - 1;
        if(length == 0) {
            return true;
        }
    } else if(!_syn_flag) {
        return false;
    } else {
        abs_seqno = unwrap(WrappingInt32(seg.header().seqno.raw_value()), WrappingInt32(_isn), abs_seqno);
        length = seg.length_in_sequence_space();
    }

    if(seg.header().fin) {
        if(_fin_flag) {
            return false;
        }
        _fin_flag = true;
        ret = true;
    } else if (seg.length_in_sequence_space() == 0 && abs_seqno == _base) {
        return true;
    } else if (abs_seqno >= _base + window_size() || abs_seqno + length <= _base) {
        if (!ret)
        {
            return false;
        }   
    }
    _reassembler.push_substring(seg.payload().copy(), abs_seqno - 1, seg.header().fin);
    _base = _reassembler.head_index() + 1;
    if (_reassembler.input_ended())
    {
        _base++;
    }
    return true;
}

optional<WrappingInt32> TCPReceiver::ackno() const {
    //  return {}; 
    if(_base > 0)
    {
        return WrappingInt32(wrap(_base, WrappingInt32(_isn)));
    } else {
        return  std::nullopt;
    }

}

size_t TCPReceiver::window_size() const {
     return _capacity - _reassembler.stream_out().buffer_size(); 
}
