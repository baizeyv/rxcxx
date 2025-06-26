//
// Created by baizeyv on 6/25/2025.
//

#include "scheduler.h"

rxcxx::scheduler::scheduler(const scheduler &src) noexcept {
    *this = src;
}

rxcxx::scheduler & rxcxx::scheduler::operator=(const scheduler &src) noexcept {
    mem = src.mem;
    return *this;
}

rxcxx::scheduler::~scheduler() noexcept = default;

void rxcxx::scheduler::abort() const noexcept {
    if (mem->interface->get_schedule_type() == scheduler_interface::schedule_type::queuing) {
        {
            std::unique_lock<std::mutex> lock(mem->mtx);
            mem->abort = true;
        }
        mem->cond.notify_one();
        mem->interface->detach();
    }
}
