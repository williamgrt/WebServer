#ifndef WEBSERVER_UTILS_SINGLETON_H_
#define WEBSERVER_UTILS_SINGLETON_H_

#include <utils/noncopyable.h>

#include <memory>
#include <pthread.h>

namespace gweb {
template<typename T>
class Singleton : noncopyable {
 public:
  static T *getInstance() {
    // 通过pthread_once机制保证只初始化一次
    pthread_once(&ponce_, &Singleton::init);
    return value_.get();
  }

 private:
  Singleton() = default;
  ~Singleton() = default;

  static void init() {
    value_ = std::make_unique<T>();
  }

  static pthread_once_t ponce_;
  static std::unique_ptr<T> value_;

};

template<typename T>
pthread_once_t Singleton<T>::ponce_ = PTHREAD_ONCE_INIT;

template<typename T>
std::unique_ptr<T> Singleton<T>::value_ = nullptr;
}

#endif //WEBSERVER_UTILS_SINGLETON_H_
