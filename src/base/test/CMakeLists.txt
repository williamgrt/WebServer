add_executable(TestThread TestThread.cc)
target_link_libraries(TestThread libbase)

add_executable(TestMultiThread TestMultiThread.cc)
target_link_libraries(TestMultiThread libbase)

add_executable(TestThreadPool TestThreadPool.cc)
target_link_libraries(TestThreadPool libbase)