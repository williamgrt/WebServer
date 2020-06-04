#include "LogBuffer.h"

using namespace web;
using namespace std;

template<int SIZE>
void LogBuffer<SIZE>::append(const char *line, std::size_t len) {
  if (available() >= len) {
    memcpy(curr_, line, len);
    curr_ += len;
  }
}