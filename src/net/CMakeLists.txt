set(NET_LIB
    Channel.cc
    EventLoop.cc
    EPoller.cc
    Socket.cc
    Timer.cc
    Acceptor.cc
    TimerQueue.cc
    Buffer.cc
    EventLoopThread.cc
    TcpServer.cc
    TcpConnection.cc
    EventLoopThreadPool.cc EventLoopThreadPool.h)

add_library(Gweb_net ${NET_LIB})

add_subdirectory(test)