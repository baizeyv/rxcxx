//
// Created by baizeyv on 6/26/2025.
//

#ifndef NATIVE_INTERVAL_OBSERVABLE_HPP
#define NATIVE_INTERVAL_OBSERVABLE_HPP
#include <chrono>
#include <bits/this_thread_sleep.h>

#include "../observable.hpp"
#include "../scheduler.h"
#include "../schedules/default_scheduler_impl.h"

namespace rxcxx::observables {
    template<typename T>
    auto native_interval(std::chrono::milliseconds msec,
                         std::function<bool(const T &)> end_cond = [](const T &) { return false; },
                         scheduler::creator_func sccr = schedulers::default_scheduler()) noexcept
        -> observable<T> {
        return observable<>::make_observable<T>([msec, sccr, end_cond](observer<T> s) {
            auto scdl = sccr();
            scdl.schedule([s, msec, scdl, end_cond]() {
                std::shared_ptr<T> n = std::make_shared<T>();
                while (s.is_no_disposed()) {
                    std::this_thread::sleep_for(msec);
                    s.on_next(*n);
                    ++(*n);
                    if (end_cond(*n)) {
                        s.on_completed();
                        break;
                    }
                }
                scdl.abort();
            });
        });
    }
}

#endif //NATIVE_INTERVAL_OBSERVABLE_HPP
