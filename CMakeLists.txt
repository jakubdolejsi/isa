cmake_minimum_required(VERSION 3.15)
project(isa)

set(CMAKE_CXX_STANDARD 14)


add_executable(isaserver isaserver.cpp server/ServerArgumentParser.cpp error/Error.h server/Server.cpp server/RequestParser.cpp server/DataProcesser.cpp Helpers/VectorMapper.cpp server/DataProcesser.h server/RequestParser.h server/Server.h server/ServerArgumentParser.h Helpers/VectorMapper.h Helpers/SignalHandler.cpp Helpers/SignalHandler.h)
add_executable(isaclient isaclient.cpp client/ClientArgumentParser.cpp error/Error.h client/Client.cpp client/ClientArgumentParser.h client/Client.h)

find_package(Threads)


target_link_libraries(isaserver ${CMAKE_THREAD_LIBS_INIT})
