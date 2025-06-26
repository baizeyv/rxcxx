//
// Created by baizeyv on 6/26/2025.
//

#ifndef FIRST_OPERATOR_HPP
#define FIRST_OPERATOR_HPP
#include "take_operator.hpp"

namespace rxcxx::operators {
    inline auto first() noexcept {
        return take(1);
    }
}

#endif //FIRST_OPERATOR_HPP
