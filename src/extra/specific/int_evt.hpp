//
// Created by baizeyv on 6/27/2025.
//

#ifndef INT_EVT_HPP
#define INT_EVT_HPP
#include "../evt_set.hpp"

namespace rxcxx::extra::specific {
    template<class T>
    class int_evt : public evt_set<int, T> {
    };
}

#endif //INT_EVT_HPP
