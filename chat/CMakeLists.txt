cmake_minimum_required(VERSION 2.6)
project(chat)

FIND_PACKAGE( Boost 1.58 COMPONENTS program_options system filesystem thread REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
link_directories(${Boost_LIBRARY_DIR})

add_executable(chatserver chat_server.cpp)
add_executable(chatclient chat_client.cpp)

target_compile_features(chatserver PRIVATE cxx_auto_type)
target_compile_features(chatclient PRIVATE cxx_auto_type)

install(TARGETS chatserver RUNTIME DESTINATION bin)
install(TARGETS chatclient RUNTIME DESTINATION bin)

TARGET_LINK_LIBRARIES( chatserver ${Boost_LIBRARIES} )
TARGET_LINK_LIBRARIES( chatclient ${Boost_LIBRARIES} )