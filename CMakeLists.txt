cmake_minimum_required(VERSION 3.19)
project(aydan_db)

set(CMAKE_CXX_STANDARD 20)

add_executable(aydan_db main.cpp binary/Stream.cpp binary/Stream.h binary/Exception.cpp binary/Exception.h type/byte.h)