//
// Created by baizeyv on 6/25/2025.
//

#include "scheduler_interface.h"

rxcxx::scheduler_interface::scheduler_interface(const schedule_type p_type) noexcept : sc_type(p_type) {
}

rxcxx::scheduler_interface::schedule_type rxcxx::scheduler_interface::get_schedule_type() const noexcept {
    return sc_type;
}
