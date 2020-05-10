set(NET_LIB
Channel.cc EventLoop.cc EPoller.cc)

add_library(lib_net ${NET_LIB})

add_subdirectory(test)