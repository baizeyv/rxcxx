//
// Created by baizeyv on 6/25/2025.
//

#ifndef EMPTY_OBSERVABLE_HPP
#define EMPTY_OBSERVABLE_HPP
#include "../observable.hpp"


namespace rxcxx::observables {
    template<typename T>
    inline auto empty() noexcept -> observable<T> {
        return observable<>::make_observable<T>([](observer<T> ob) {
            ob.on_completed();
        });
    }
}


#endif //EMPTY_OBSERVABLE_HPP
