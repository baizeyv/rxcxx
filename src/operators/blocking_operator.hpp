//
// Created by baizeyv on 6/26/2025.
//

#ifndef BLOCKING_OPERATOR_HPP
#define BLOCKING_OPERATOR_HPP
#include "../observables/blocking_observables.hpp"

namespace rxcxx::operators {
    inline auto block() noexcept {
        return [](auto src) {
            using OUT_OB = decltype(src);
            using OUT = typename OUT_OB::value_type;
            return observables::blocking<>::make_blocking<OUT>(src);
        };
    }
}

#endif //BLOCKING_OPERATOR_HPP
