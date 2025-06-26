//
// Created by baizeyv on 6/26/2025.
//

#ifndef LAST_OPERATOR_HPP
#define LAST_OPERATOR_HPP
#include "take_last_operator.hpp"

namespace rxcxx::operators {
    inline auto last() noexcept {
        return take_last(1);
    }
}

#endif //LAST_OPERATOR_HPP
