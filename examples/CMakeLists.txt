add_definitions(-std=c++11)

add_executable(Echo Echo.cc)
target_link_libraries(Echo Gweb_net Gweb_base)
