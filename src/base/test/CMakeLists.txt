add_executable(test_thread test_thread.cc)
target_link_libraries(test_thread server_base)

add_executable(test_mutex test_mutex.cc)
target_link_libraries(test_mutex server_base)