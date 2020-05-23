//
// Created by 郭睿韬 on 2020/5/23.
//

#include "../Acceptor.h"
#include "../Socket.h"
#include "../EventLoop.h"
#include <cstdio>

using namespace web;
using namespace std;

void newConnFunction(Socket &&socket, Ip4Addr addr) {
  ::write(socket.fd(), "How are you?\n", 13);
  socket.close();
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: ./TestAcceptor --port");
  }

  int port = atoi(argv[1]);
  EventLoop loop;
  Acceptor acceptor(&loop, "", port);
  acceptor.setNewConnCallBack(newConnFunction);
  acceptor.listen();

  loop.loop();

}