#ifndef LXLOOKOUTRECORD_LIBLOOKOUT_TUN_HH
#define LXLOOKOUTRECORD_LIBLOOKOUT_TUN_HH

#include "file_descriptor.hh"

#include <string>

//! A FileDescriptor to a [Linux TUN] device
class TunFD : public FileDescriptor {
  public:
    //! Open an existing persistent [TUN device].
    explicit TunFD(const std::string &devname);
};

#endif
