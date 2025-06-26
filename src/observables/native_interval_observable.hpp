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
    template<typename T = int>
    auto interval(std::chrono::milliseconds msec,
                  scheduler::creator_func sccr = schedulers::default_scheduler()) noexcept
        -> observable<T> {
        return observable<>::make_observable<T>([msec, sccr](observer<T> s) {
            auto scdl = sccr();
            scdl.schedule([s, msec, scdl]() {
                T n = 1;
                while (s.is_subscribed()) {
                    std::this_thread::sleep_for(msec);
                    s.on_next(n);
                    n++;
                }
                scdl.abort();
            });
        });
    }
}

#endif //NATIVE_INTERVAL_OBSERVABLE_HPP
