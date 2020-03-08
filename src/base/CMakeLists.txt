set(BASE_LIB
    thread.cc mutex.h condvar.h)

add_library(server_base ${BASE_LIB})

add_subdirectory(test)