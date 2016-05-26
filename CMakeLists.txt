cmake_minimum_required(VERSION 2.6)
project(msgqueue)

FIND_PACKAGE( Boost 1.58 COMPONENTS program_options system filesystem thread REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
link_directories(${Boost_LIBRARY_DIR})

add_executable(msgqueue main.cpp)
add_executable(timeclient timeclient.cpp)
add_executable(timeserver timeserver.cpp)
add_executable(chatserver chat_server.cpp)
add_executable(chatclient chat_client.cpp)

target_compile_features(timeclient PRIVATE cxx_auto_type)
target_compile_features(timeserver PRIVATE cxx_auto_type)
target_compile_features(chatserver PRIVATE cxx_auto_type)
target_compile_features(chatclient PRIVATE cxx_auto_type)

install(TARGETS msgqueue RUNTIME DESTINATION bin)
TARGET_LINK_LIBRARIES( msgqueue ${Boost_LIBRARIES} )
TARGET_LINK_LIBRARIES( timeclient ${Boost_LIBRARIES} )
TARGET_LINK_LIBRARIES( timeserver ${Boost_LIBRARIES} )
TARGET_LINK_LIBRARIES( chatserver ${Boost_LIBRARIES} )
TARGET_LINK_LIBRARIES( chatclient ${Boost_LIBRARIES} )