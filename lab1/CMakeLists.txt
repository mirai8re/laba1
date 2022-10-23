cmake_minimum_required(VERSION 3.23)
project (creator_cpp)

set(CMAKE_CXX_STANDARD 98)
add_executable(creator creator.cpp employee.h )
add_executable(reporter reporter.cpp employee.h )
add_executable(main main.cpp employee.h )

