#ifndef _UTILS_H
#define _UTILS_H

// some frequently used header files
#include <cassert>
#include <cerrno>
#include <error.h>
#include <fcntl.h>
#include <unistd.h>
#include <memory>
#include <functional>

#include <gnet/utils/noncopyable.h>

namespace gweb {
class TcpConnection;
class Buffer;

// 指向TCP连接的智能指针类型
using TcpConnPtr = std::shared_ptr<TcpConnection>;

// 建立连接的回调函数类型
using ConnectionCallback = std::function<void(const TcpConnPtr &)>;

// 接收到消息后调用的回调函数
using MessageCallback = std::function<void(const TcpConnPtr,
                                           Buffer *)>;

// 关闭连接时的回调函数
using CloseCallback = std::function<void(const TcpConnPtr &)>;

// 写完成后调用的回调函数
using WriteCompleteCallback = std::function<void(const TcpConnPtr &)>;

}

#endif // _UTILS_H