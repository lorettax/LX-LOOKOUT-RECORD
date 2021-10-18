file (GLOB LIB_SOURCES "*.cc" "util/*.cc" "tcp_helpers/*.cc")
add_library (lx-lookout-record STATIC ${LIB_SOURCES})
