#ifndef WEBSERVER_SRC_BASE_FILE_H
#define WEBSERVER_SRC_BASE_FILE_H

#include <string>
#include <mutex>
#include <memory>
#include "noncopyable.h"
namespace web {

class File : noncopyable {
public:
  File(const std::string &name, int flushEveryNum);
  ~File() = default;
  void write(const char *line, std::size_t len);

private:
  class Impl {
  public:
    explicit Impl(const std::string &fileName);
    ~Impl();

    void append(const char *line, std::size_t len);

  private:
    int fd_;
  };

  std::string fileName_;
  int flushEveryNum_;
  std::unique_ptr<Impl> file_;
};

}

#endif //WEBSERVER_SRC_BASE_FILE_H
