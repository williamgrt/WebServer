set(NET_LIB
TcpServer.cc Channel.cc EventLoop.cc Poller.cc Socket.cc)

add_library(server_net ${NET_LIB})