cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Weverything -Werror -Wno-c++98-compat-pedantic -Wno-c++98-compat -Wno-missing-prototypes -Wno-unused-macros ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_executable(day1 day1/main.cpp)
