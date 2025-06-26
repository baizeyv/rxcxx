//
// Created by baizeyv on 6/25/2025.
//

#ifndef RANGE_OBSERVABLE_HPP
#define RANGE_OBSERVABLE_HPP
#include "../observable.hpp"
#include "../rxcxx-utils.h"

namespace rxcxx::observables {
    template<typename T>
    auto range(T start, T end) noexcept -> observable<typename strip_const_ref<T>::type> {
        using TT = typename strip_const_ref<T>::type;
        return observable<>::make_observable<TT>([start, end](observer<TT> ob) {
            for (TT i = start; i <= end; ++i) {
                if (!ob.is_no_disposed())
                    return;
                ob.on_next(i);
            }
            ob.on_completed();
        });
    }
}

#endif //RANGE_OBSERVABLE_HPP
