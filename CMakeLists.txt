cmake_minimum_required(VERSION 3.31)
project(rxcxx)

set(CMAKE_CXX_STANDARD 17)

add_executable(rxcxx main.cpp
        src/rxcxx.h
        src/extra/reactive_variable.hpp
        src/extra/simple_event.hpp
        src/extra/simple_message.h
        src/extra/int_message.h
        neo/rxcxx-observable.hpp
)
