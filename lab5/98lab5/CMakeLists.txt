cmake_minimum_required(VERSION 3.23)
project(Lab5__98_)

set(CMAKE_CXX_STANDARD 98)

add_executable(server server.cpp employee.h constants.h errors.h)
add_executable(client client.cpp employee.h constants.h errors.h entryState.h)
