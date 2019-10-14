cmake_minimum_required(VERSION 3.15)
project(isa)

set(CMAKE_CXX_STANDARD 14)

add_executable(isaserver isaserver.cpp server/ServerArgumentParser.cpp error/Error.h server/Server.cpp server/RequestParser.cpp server/Data.cpp Helpers/VectorMapper.cpp server/Data.h server/RequestParser.h server/Server.h server/ServerArgumentParser.h Helpers/VectorMapper.h)
add_executable(isaclient isaclient.cpp client/ClientArgumentParser.cpp error/Error.h client/Client.cpp client/ClientArgumentParser.h client/Client.h)