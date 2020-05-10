set(BASE_LIB
    noncopyable.h
    Thread.cc
    Mutex.cc
    Condition.cc
    ThreadPool.cc
    )

add_library(libbase ${BASE_LIB})
target_link_libraries(libbase pthread rt)

add_subdirectory(test)