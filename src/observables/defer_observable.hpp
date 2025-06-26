//
// Created by baizeyv on 6/26/2025.
//

#ifndef DEFER_OBSERVABLE_HPP
#define DEFER_OBSERVABLE_HPP
#include "../observable.hpp"
#include "../scheduler.h"
#include "../schedules/default_scheduler_impl.h"

namespace rxcxx::observables {
    template<typename F>
    inline auto defer(F &&fn, scheduler::creator_func sccr = schedulers::default_scheduler()) noexcept
        -> decltype(fn()) {
        using Source = decltype(fn());
        using Item = typename Source::value_type;
        return observable<>::make_observable<Item>([fn, sccr](observer<Item> s) {
            auto sctl = stream_controller<Item>(s);
            auto scdl = sccr();
            sctl.set_on_finalize([scdl] {
                scdl.abort();
            });
            scdl.schedule([fn, sctl]() {
                fn().subscribe(sctl.template make_observer<Item>(
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
    }
}

#endif //DEFER_OBSERVABLE_HPP
