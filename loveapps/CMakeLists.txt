add_library (stream_copy STATIC bidirectional_stream_copy.cc)

add_lookout_exec (udp_tcpdump ${LIBPCAP})
add_lookout_exec (tcp_native stream_copy)
add_lookout_exec (tun)
add_lookout_exec (tcp_udp stream_copy)
add_lookout_exec (tcp_ipv4 stream_copy)
add_lookout_exec (loveget)
add_lookout_exec (tcp_benchmark)
