#include <iostream>
#include "../TcpServer.h"
#include "../EventLoop.h"
#include "../HeartBeatTcpServer.h"

using namespace web;
using namespace std;

void onConnection(const TcpConnectionPtr &conn) {
  if (conn->connected()) {
    cout << "onConnection(): new connection [" << conn->name() << "] from " << conn->getPeerAddr().getHostPort()
         << "in thread " << this_thread::get_id() << "\n";
  } else {
    cout << "onConnection(): connection [" << conn->name() << "] is down\n";
  }
}

void onMessage(const TcpConnectionPtr &conn, Buffer *buffer) {
  conn->send(buffer->retrieveAsString());
}

int main(int argc, char **argv) {
  printf("main(): pid = %d\n", getpid());
  EventLoop loop;
  unsigned int port = 12345;
  HeartBeatTcpServer server(&loop, "", port, 10);
  // TcpServer server(&loop, "", port);
  int threadNum = atoi(argv[1]);

  server.setThreadNum(threadNum);
  server.setConnectionCallback(onConnection);
  server.setMessageCallback(onMessage);
  server.start();
  loop.loop();
}
