//
// Created by baizeyv on 6/25/2025.
//

#ifndef JUST_OBSERVABLE_HPP
#define JUST_OBSERVABLE_HPP
#include "../observable.hpp"
#include "../rxcxx-utils.h"
#include "../scheduler.h"
#include "../schedules/default_scheduler_impl.h"

namespace rxcxx::observables {
    template<typename T>
    inline auto just(T &&value, scheduler::creator_func sccr = schedulers::default_scheduler()) noexcept
        -> observable<typename strip_const_ref<T>::type> {
        using TT = typename strip_const_ref<T>::type;
        auto v = std::make_shared<TT>(std::forward<T>(value));
        return observable<>::make_observable<TT>([v, sccr](observer<TT> ob) {
            auto scdl = sccr();
            scdl.schedule([v, ob, scdl]() {
                ob.on_next(*v);
                ob.on_completed();
                scdl.abort();
            });
        });
    }
}

#endif //JUST_OBSERVABLE_HPP
