set(NET_LIB
    Channel.cc
    EventLoop.cc
    EPoller.cc
    Socket.cc
    Timer.cc
    Acceptor.cc
    TimerQueue.cc
    Dispatcher.h)

add_library(Gweb_net ${NET_LIB})

add_subdirectory(test)