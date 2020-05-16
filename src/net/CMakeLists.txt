set(NET_LIB
Channel.cc EventLoop.cc EPoller.cc Socket.cc Timer.cc Timer.h Acceptor.cc Acceptor.h TimerQueue.cc)

add_library(lib_net ${NET_LIB})

add_subdirectory(test)