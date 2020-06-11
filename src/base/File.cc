#include <unistd.h>
#include <fcntl.h>
#include "File.h"

using namespace web;
using namespace std;

File::File(const std::string &name, int flushEveryNum) :
    fileName_(name),
    flushEveryNum_(flushEveryNum) {
  file_.reset(new Impl(fileName_));
}

void File::write(const char *line, std::size_t len) {
  file_->append(line, len);
}

File::Impl::Impl(const string &fileName) :
    fd_(open(fileName.c_str(), O_RDWR | O_CREAT | O_APPEND, (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH))) {
}

File::Impl::~Impl() {
  close(fd_);
}

void File::Impl::append(const char *line, std::size_t len) {
  std::size_t n = ::write(fd_, line, len);
  std::size_t remain = len - n;
  while (remain > 0) {
    size_t x = ::write(fd_, line + n, remain);
    if (x == -1) {
      perror("write error");
      break;
    }
    n += x;
    remain = len - n;
  }
}
