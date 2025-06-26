//
// Created by baizeyv on 6/26/2025.
//

#ifndef SUBSCRIBE_ON_OPERATOR_HPP
#define SUBSCRIBE_ON_OPERATOR_HPP
#include "../observable.hpp"
#include "../scheduler.h"

namespace rxcxx::operators {
    inline auto subscribe_on(scheduler::creator_func sccr) noexcept {
        return [sccr](auto source) {
            using Source = decltype(source);
            using Item = typename Source::value_type;
            return observable<>::make_observable<Item>([source, sccr](observer<Item> s) {
                auto sctl = stream_controller<Item>(s);
                auto scdl = sccr();
                sctl.set_on_finalize([scdl] {
                    scdl.abort();
                });
                scdl.schedule([sctl, source]() {
                    source.subscribe(sctl.template make_observer<Item>(
                        [sctl](auto, const Item &x) {
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

#endif //SUBSCRIBE_ON_OPERATOR_HPP
