cmake_minimum_required(VERSION 3.15)
project(isa)

set(CMAKE_CXX_STANDARD 14)

add_executable(isaserver isaserver.cpp ServerArgumentParser.h Error.h Server.h)
add_executable(isaclient isaclient.cpp ClientArgumentParser.h Error.h Client.h)