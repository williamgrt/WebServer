set(NET_LIB
    Channel.cc
    EventLoop.cc
    Epoll.cc
    Socket.cc
    Timer.cc
    Acceptor.cc
    TimerQueue.cc
    Buffer.cc
    EventLoopThread.cc
    TcpServer.cc
    TcpConnection.cc
    EventLoopThreadPool.cc
    HeartBeatTcpServer.cc)

add_library(Gweb_net ${NET_LIB})
target_link_libraries(Gweb_net Gweb_log Gweb_base)

add_subdirectory(test)