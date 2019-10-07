cmake_minimum_required(VERSION 3.15)
project(isa)

set(CMAKE_CXX_STANDARD 14)

add_executable(isaserver isaserver.cpp server/ServerArgumentParser.h error/Error.h server/Server.h server/RequestParser.h server/Datas.h)
add_executable(isaclient isaclient.cpp client/ClientArgumentParser.h error/Error.h client/Client.h)