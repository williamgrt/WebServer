//
// Created by 郭睿韬 on 2020/5/23.
//

#include "net/Acceptor.h"
#include "net/Socket.h"
#include "net/EventLoop.h"
#include <cstdio>

using namespace gweb;
using namespace std;

void newConnFunction(Socket &&socket, Ip4Addr addr) {
  ::write(socket.fd(), "How are you?\n", 13);
  socket.close();
}

int main(int argc, char **argv) {
  int port = 12345;
  EventLoop loop;
  Acceptor acceptor(&loop, "", port);
  acceptor.setNewConnCallBack(newConnFunction);
  acceptor.listen();

  loop.loop();

}