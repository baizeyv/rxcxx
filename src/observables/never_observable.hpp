//
// Created by baizeyv on 6/25/2025.
//

#ifndef NEVER_OBSERVABLE_H
#define NEVER_OBSERVABLE_H
#include "../observable.hpp"

namespace rxcxx::observables {
    template<typename T>
    inline auto never() noexcept -> observable<T> {
        return observable<>::make_observable<T>([](observer<T> ob) {
        });
    }
}

#endif //NEVER_OBSERVABLE_H
