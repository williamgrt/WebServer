set(BASE_LIB
    noncopyable.h
    Thread.cc
    Mutex.cc
    Condition.cc
    ThreadPool.cc
    Logger.cc
    LogBuffer.cc
    MmapFile.cc CountDownLatch.cc Log.cc Log.h LogStream.cc LogStream.h)

add_definitions(-std=c++11)
add_library(Gweb_base ${BASE_LIB})
target_link_libraries(Gweb_base pthread rt)

add_subdirectory(test)