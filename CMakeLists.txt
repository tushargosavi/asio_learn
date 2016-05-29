cmake_minimum_required(VERSION 2.6)
project(msgqueue)

FIND_PACKAGE( Boost 1.58 COMPONENTS program_options system filesystem thread REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
link_directories(${Boost_LIBRARY_DIR})

add_executable(msgqueue main.cpp)


install(TARGETS msgqueue RUNTIME DESTINATION bin)
TARGET_LINK_LIBRARIES( msgqueue ${Boost_LIBRARIES} )

add_subdirectory(timeserver)
add_subdirectory(chat)
add_subdirectory(http1)
