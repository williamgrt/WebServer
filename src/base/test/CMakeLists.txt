add_executable(TestThread TestThread.cc)
target_link_libraries(TestThread Gweb_base)

add_executable(TestMultiThread TestMultiThread.cc ../MmapFile.cc ../MmapFile.h)
target_link_libraries(TestMultiThread Gweb_base)

add_executable(TestThreadPool TestThreadPool.cc)
target_link_libraries(TestThreadPool Gweb_base)