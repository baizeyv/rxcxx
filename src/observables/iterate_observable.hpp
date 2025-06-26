//
// Created by baizeyv on 6/26/2025.
//

#ifndef ITERATE_OBSERVABLE_HPP
#define ITERATE_OBSERVABLE_HPP
#include "../observable.hpp"

namespace rxcxx::observables {
    template<typename T>
    auto iterate(T arr) noexcept
        -> observable<typename T::value_type> {
        using TT = typename T::value_type;
        return observable<>::make_observable<TT>([arr](observer<TT> s) {
            for (auto it = std::cbegin(arr); it != std::cend(arr); it++) {
                if (!s.is_no_disposed()) return;
                s.on_next(*it);
            }
            s.on_completed();
        });
    }
}

#endif //ITERATE_OBSERVABLE_HPP
