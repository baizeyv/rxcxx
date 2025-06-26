//
// Created by baizeyv on 6/26/2025.
//

#ifndef NATIVE_DELAY_OPERATOR_HPP
#define NATIVE_DELAY_OPERATOR_HPP
#include <chrono>
#include <bits/this_thread_sleep.h>

#include "../observable.hpp"
#include "../scheduler.h"
#include "../schedules/default_scheduler_impl.h"

namespace rxcxx::operators {

    inline auto native_delay(std::chrono::milliseconds msec,
                      scheduler::creator_func sccr = schedulers::default_scheduler()) noexcept {
        return [msec, sccr](auto source) {
            using Source = decltype(source);
            using Item = typename Source::value_type;
            return observable<>::make_observable<Item>([source, msec, sccr](observer<Item> s) {
                auto sctl = stream_controller<Item>(s);
                auto scdl = sccr();
                sctl.set_on_finalize([scdl] {
                    scdl.abort();
                });
                scdl.schedule([source, msec, sctl]() {
                    source.subscribe(sctl.template make_observer<Item>(
                        [sctl, msec](auto, const Item &x) {
                            std::this_thread::sleep_for(msec);
                            sctl.sink_next(x);
                        },
                        [sctl](auto, std::exception_ptr err) {
                            sctl.sink_error(err);
                        },
                        [sctl](auto serial) {
                            sctl.sink_completed(serial);
                        }
                    ));
                });
            });
        };
    }
}

#endif //NATIVE_DELAY_OPERATOR_HPP
