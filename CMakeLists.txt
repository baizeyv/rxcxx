﻿cmake_minimum_required(VERSION 3.31)
project(rxcxx)

set(CMAKE_CXX_STANDARD 17)

add_executable(rxcxx main.cpp
        memleak.h
        utils.h
        src/observable.hpp
        src/observer.hpp
        src/subscription.hpp
        src/observables/empty_observable.hpp
        src/observables/never_observable.hpp
        src/observables/error_observable.hpp
        src/scheduler.cpp
        src/scheduler.h
        src/scheduler_interface.cpp
        src/scheduler_interface.h
        src/schedules/default_scheduler_impl.cpp
        src/schedules/default_scheduler_impl.h
        src/schedules/new_thread_scheduler_impl.cpp
        src/schedules/new_thread_scheduler_impl.h
        src/rxcxx-utils.h
        src/observables/range_observable.hpp
        src/observables/just_observable.hpp
        src/stream_controller.hpp
        src/operators/take_operator.hpp
        src/operators/subscribe_operator.hpp
        src/rxcxx.h
        src/operators/take_while_operator.hpp
        src/operators/take_until_operator.hpp
        src/operators/take_last_operator.hpp
        src/operators/last_operator.hpp
        src/operators/first_operator.hpp
        src/operators/where_operator.hpp
        src/operators/select_operator.hpp
        src/operators/finally_operator.hpp
        src/operators/skip_while_operator.hpp
        src/operators/skip_operator.hpp
        src/operators/distinct_until_changed_operator.hpp
        src/observables/connectable_observable.hpp
        src/subjects/subject.hpp
        src/operators/on_error_resume_next_operator.hpp
        src/operators/publish_operator.hpp
        src/subjects/behavior.hpp
        src/subjects/replay.hpp
        src/operators/tap_operator.hpp
        src/operators/skip_until_operator.hpp
        src/operators/subscribe_on_operator.hpp
        src/operators/observe_on_operator.hpp
        src/operators/retry_operator.hpp
        src/operators/native_delay_operator.hpp
        src/operators/merge_operator.hpp
        src/operators/amb_operator.hpp
        src/observables/native_interval_observable.hpp
        src/observables/iterate_observable.hpp
        src/observables/defer_observable.hpp
        src/observables/blocking_observables.hpp
        src/operators/blocking_operator.hpp
        src/operators/select_many_operator.hpp
        src/operators/zip_operator.hpp
        src/operators/timeout_operator.hpp
        src/extra/rx_variable.hpp
        src/extra/rx_event.hpp
        src/extra/rx_message.hpp
        src/extra/unit.h
        src/extra/specific/int_msg.hpp
        src/extra/msg_set.hpp
        src/extra/evt_set.hpp
        src/extra/specific/int_evt.hpp
        src/extra/specific/string_msg.hpp
        src/extra/specific/string_evt.hpp
        utils/file_utility.cpp
        utils/file_utility.h
)
#add_library(rxcxx SHARED main.cpp
#        memleak.h
#        utils.h
#        src/observable.hpp
#        src/observer.hpp
#        src/subscription.hpp
#        src/observables/empty_observable.hpp
#        src/observables/never_observable.hpp
#        src/observables/error_observable.hpp
#        src/scheduler.cpp
#        src/scheduler.h
#        src/scheduler_interface.cpp
#        src/scheduler_interface.h
#        src/schedules/default_scheduler_impl.cpp
#        src/schedules/default_scheduler_impl.h
#        src/schedules/new_thread_scheduler_impl.cpp
#        src/schedules/new_thread_scheduler_impl.h
#        src/rxcxx-utils.h
#        src/observables/range_observable.hpp
#        src/observables/just_observable.hpp
#        src/stream_controller.hpp
#        src/operators/take_operator.hpp
#        src/operators/subscribe_operator.hpp
#        src/rxcxx.h
#        src/operators/take_while_operator.hpp
#        src/operators/take_until_operator.hpp
#        src/operators/take_last_operator.hpp
#        src/operators/last_operator.hpp
#        src/operators/first_operator.hpp
#        src/operators/where_operator.hpp
#        src/operators/select_operator.hpp
#        src/operators/finally_operator.hpp
#        src/operators/skip_while_operator.hpp
#        src/operators/skip_operator.hpp
#        src/operators/distinct_until_changed_operator.hpp
#        src/observables/connectable_observable.hpp
#        src/subjects/subject.hpp
#        src/operators/on_error_resume_next_operator.hpp
#        src/operators/publish_operator.hpp
#        src/subjects/behavior.hpp
#        src/subjects/replay.hpp
#        src/operators/tap_operator.hpp
#        src/operators/skip_until_operator.hpp
#        src/operators/subscribe_on_operator.hpp
#        src/operators/observe_on_operator.hpp
#        src/operators/retry_operator.hpp
#        src/operators/native_delay_operator.hpp
#        src/operators/merge_operator.hpp
#        src/operators/amb_operator.hpp
#        src/observables/native_interval_observable.hpp
#        src/observables/iterate_observable.hpp
#        src/observables/defer_observable.hpp
#        src/observables/blocking_observables.hpp
#        src/operators/blocking_operator.hpp
#        src/operators/select_many_operator.hpp
#        src/operators/zip_operator.hpp
#        src/operators/timeout_operator.hpp
#        src/extra/rx_variable.hpp
#        src/extra/rx_event.hpp
#        src/extra/rx_message.hpp
#        src/extra/unit.h
#        src/extra/specific/int_msg.hpp
#        src/extra/msg_set.hpp
#        src/extra/evt_set.hpp
#        src/extra/specific/int_evt.hpp
#        src/extra/specific/string_msg.hpp
#        src/extra/specific/string_evt.hpp
#)
#target_compile_definitions(rxcxx PRIVATE RXCXX_EXPORTS)
#target_compile_options(rxcxx PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/O2 /GL>)
#target_link_options(rxcxx PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/OPT:REF /OPT:ICF /LTCG>)