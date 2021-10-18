#ifndef LOOKOUT_LOVEAPPS_BIDIRECTIONAL_STREAM_COPY_HH
#define LOOKOUT_LOVEAPPS_BIDIRECTIONAL_STREAM_COPY_HH

#include "socket.hh"

//! Copy socket input/output to stdin/stdout until finished
void bidirectional_stream_copy(Socket &socket);

#endif 
