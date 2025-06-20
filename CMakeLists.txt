cmake_minimum_required(VERSION 3.31)
project(rxcxx)

set(CMAKE_CXX_STANDARD 17)

add_executable(rxcxx main.cpp
        src/base/disposable.h
        src/base/abs_observable.hpp
        src/base/abs_observer.hpp
        src/base/single_assignment_disposable.cpp
        src/base/single_assignment_disposable.h
        src/base/disposed_sentinel.cpp
        src/base/disposed_sentinel.h
        src/base/empty_disposable.h
        src/base/result.cpp
        src/base/result.h
        src/base/stubs.h
        src/base/scope_guard.cpp
        src/base/scope_guard.h
        src/base/observer_node.hpp
        src/base/observer_node_parent.hpp
        src/variable/readonly_reactive_variable.hpp
        src/variable/reactive_variable.hpp
        src/base/subject.hpp
        src/base/anonymous_observer.hpp
        src/memleak.h
        src/event/simple_event.hpp
        src/base/unit.cpp
        src/base/unit.h
        src/event/simple_message.h
        src/event/int_event.hpp
        src/event/int_message.h
        src/event/int_message.cpp
        src/base/stubs.cpp
        src/event/basic_event.hpp
        src/event/string_event.hpp
        src/event/string_message.cpp
        src/event/string_message.h
        src/event/basic_message.h
        src/operator/skip.hpp
        src/operator/operator_factory.h
        src/operator/take.hpp
        src/utils.h
        src/operator/where.hpp
        src/operator/skip_while.hpp
        src/operator/take_while.hpp
        src/operator/distinct.hpp
        src/operator/do.hpp
)
