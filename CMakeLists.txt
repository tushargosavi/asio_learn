cmake_minimum_required(VERSION 2.6)
project(msgqueue)

add_executable(msgqueue main.cpp)

install(TARGETS msgqueue RUNTIME DESTINATION bin)
