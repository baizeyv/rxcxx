//
// Created by baizeyv on 6/26/2025.
//

#ifndef OBSERVE_ON_OPERATOR_HPP
#define OBSERVE_ON_OPERATOR_HPP
#include "../observable.hpp"
#include "../scheduler.h"

namespace rxcxx::operators {
    // ! 目前只支持到observe_on (new_thread_scheduler)
    inline auto observe_on(scheduler::creator_func sccr) noexcept {
        return [sccr](auto source) {
            using Source = decltype(source);
            using Item = typename Source::value_type;
            return observable<>::make_observable<Item>([source, sccr](observer<Item> s) {
                auto sctl = stream_controller<Item>(s);
                auto scdl = sccr();
                sctl.set_on_finalize([scdl] {
                    scdl.abort();
                });
                source.subscribe(sctl.template make_observer<Item>(
                    [sctl, scdl](auto, const Item &x) {
                        scdl.schedule([sctl, x]() mutable {
                            sctl.sink_next(std::move(x));
                        });
                    },
                    [sctl, scdl](auto, std::exception_ptr err) {
                        scdl.schedule([sctl, err]() {
                            sctl.sink_error(err);
                        });
                    },
                    [sctl, scdl](auto serial) {
                        scdl.schedule([sctl, serial]() {
                            sctl.sink_completed(serial);
                        });
                    }
                ));
            });
        };
    }
}

#endif //OBSERVE_ON_OPERATOR_HPP
