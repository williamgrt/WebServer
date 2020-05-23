add_definitions(-std=c++11)

add_executable(TestLoop TestLoop.cc)
target_link_libraries(TestLoop Gweb_net Gweb_base)

add_executable(TestTimer TestTimer.cc)
target_link_libraries(TestTimer Gweb_net Gweb_base)

add_executable(TestRunInLoop TestRunInLoop.cc)
target_link_libraries(TestRunInLoop Gweb_net Gweb_base)

add_executable(TestAcceptor TestAcceptor.cc)
target_link_libraries(TestAcceptor Gweb_net Gweb_base)