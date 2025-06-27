//
// Created by baizeyv on 6/27/2025.
//

#ifndef RX_VARIABLE_HPP
#define RX_VARIABLE_HPP
#include "../subjects/behavior.hpp"

namespace rxcxx::extra {
    template<class T>
    class rx_variable {
    public:
        using value_type = T;
    private:
        subjects::behavior<value_type> behavior;
    };
}

#endif //RX_VARIABLE_HPP
