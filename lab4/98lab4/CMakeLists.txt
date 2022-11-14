cmake_minimum_required(VERSION 3.23)
project(98lab4)

set(CMAKE_CXX_STANDARD 98)

add_executable(Receiver receiver.cpp processMessage.h errors.h constants.h)
add_executable(Sender sender.cpp processMessage.h errors.h constants.h)
