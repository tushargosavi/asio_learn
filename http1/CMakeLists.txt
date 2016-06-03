cmake_minimum_required(VERSION 2.6)
project(http1)

FIND_PACKAGE( Boost 1.58 COMPONENTS program_options system filesystem thread REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
link_directories(${Boost_LIBRARY_DIR})

add_executable(http1 main.cpp server.cpp)

target_compile_features(http1 PRIVATE cxx_auto_type)

install(TARGETS http1 RUNTIME DESTINATION bin)
TARGET_LINK_LIBRARIES( http1 ${Boost_LIBRARIES} )
