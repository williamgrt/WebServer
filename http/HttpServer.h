#ifndef WEBSERVER_HTTP_HTTPSERVER_H_
#define WEBSERVER_HTTP_HTTPSERVER_H_

#include <src/net/HeartBeatTcpServer.h>
#include <src/utils/ThreadPool.h>

using namespace gweb;
using namespace std;

namespace http {
// HTTP服务器
class HttpServer: noncopyable {
 public:
  HttpServer();
  ~HttpServer() = default;



 private:
  ThreadPool pool_;
  HeartBeatTcpServer server_;
  int timeout_;
};

}

#endif //WEBSERVER_HTTP_HTTPSERVER_H_
