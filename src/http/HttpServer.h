#ifndef WEBSERVER_SRC_HTTP_HTTPSERVER_H
#define WEBSERVER_SRC_HTTP_HTTPSERVER_H

#include <memory>
namespace web {
class Buffer;
class HeartBeatTcpServer;

class HttpServer {
public:
  HttpServer();
  ~HttpServer();

  void setIOThreadNum(int ioThreadNum);
  void start();

private:
  std::unique_ptr<HeartBeatTcpServer> server_;

};

}


#endif //WEBSERVER_SRC_HTTP_HTTPSERVER_H
