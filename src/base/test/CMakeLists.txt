add_executable(TestThreadPool TestThreadPool.cc)
target_link_libraries(TestThreadPool Gweb_base)

add_executable(TestLog TestLog.cc)
target_link_libraries(TestLog Gweb_base)