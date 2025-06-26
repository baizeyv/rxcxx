//
// Created by baizeyv on 6/25/2025.
//

#include "default_scheduler_impl.h"

rxcxx::schedulers::default_scheduler_impl::default_scheduler_impl() noexcept : scheduler_interface(schedule_type::direct) {
}

rxcxx::schedulers::default_scheduler_impl::~default_scheduler_impl() = default;

void rxcxx::schedulers::default_scheduler_impl::run(call_in_context_func call_in_context) noexcept {
    call_in_context();
}

void rxcxx::schedulers::default_scheduler_impl::schedule(const function_type &f) noexcept {
    f();
}

void rxcxx::schedulers::default_scheduler_impl::detach() noexcept {
}
