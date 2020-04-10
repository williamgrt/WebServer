#ifndef _UTILS_H
#define _UTILS_H

// some frequently used header files
#include <cassert>
#include <cerrno>
#include <error.h>
#include <fcntl.h>
#include <unistd.h>

// typedefs
#include "TcpConnection.h"
#include "TcpServer.h"
#include <memory>

namespace gnet {
namespace types {
using TcpServerPtr = std::shared_ptr<TcpServer>;
using TcpConnPtr = std::shared_ptr<TcpConnection>;
using ChannelPtr = std::shared_ptr<Channel>;
using PollerPtr = std::shared_ptr<Poller>;

} // namespace types

} // namespace gnet

#endif // _UTILS_H