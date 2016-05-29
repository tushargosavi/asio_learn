cmake_minimum_required(VERSION 2.6)
project(timeserver)

FIND_PACKAGE( Boost 1.58 COMPONENTS program_options system filesystem thread REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
link_directories(${Boost_LIBRARY_DIR})

add_executable(timeclient timeclient.cpp)
add_executable(timeserver timeserver.cpp)

target_compile_features(timeclient PRIVATE cxx_auto_type)
target_compile_features(timeserver PRIVATE cxx_auto_type)

TARGET_LINK_LIBRARIES( timeclient ${Boost_LIBRARIES} )
TARGET_LINK_LIBRARIES( timeserver ${Boost_LIBRARIES} )

