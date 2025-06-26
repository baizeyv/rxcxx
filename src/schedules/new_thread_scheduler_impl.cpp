//
// Created by baizeyv on 6/25/2025.
//

#include "new_thread_scheduler_impl.h"

rxcxx::schedulers::new_thread_scheduler_impl::new_thread_scheduler_impl() noexcept : scheduler_interface(schedule_type::queuing) {
}

rxcxx::schedulers::new_thread_scheduler_impl::~new_thread_scheduler_impl() noexcept = default;

void rxcxx::schedulers::new_thread_scheduler_impl::run(call_in_context_func call_in_context) noexcept {
    thread = std::thread([call_in_context]() {
        call_in_context();
    });
}

void rxcxx::schedulers::new_thread_scheduler_impl::schedule(const function_type &f) noexcept {
    f();
}

void rxcxx::schedulers::new_thread_scheduler_impl::detach() noexcept {
    thread.detach();
}
