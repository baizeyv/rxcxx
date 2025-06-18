cmake_minimum_required(VERSION 3.31)
project(rxcxx)

set(CMAKE_CXX_STANDARD 17)

add_executable(rxcxx main.cpp
        src/disposable.h
        src/abs_observable.hpp
        src/abs_observer.hpp
        src/single_assignment_disposable.cpp
        src/single_assignment_disposable.h
        src/disposed_sentinel.cpp
        src/disposed_sentinel.h
        src/empty_disposable.h
        src/result.cpp
        src/result.h
        src/Stubs.h
        src/scope_guard.cpp
        src/scope_guard.h
        src/observer_node.hpp
        src/observer_node_parent.hpp
        src/readonly_reactive_variable.hpp
        src/reactive_variable.hpp
        src/subject.hpp
        src/anonymous_observer.hpp
        src/memleak.h
)
