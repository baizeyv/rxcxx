//
// Created by baizeyv on 6/27/2025.
//

#ifndef STRING_EVT_HPP
#define STRING_EVT_HPP
#include "../evt_set.hpp"

namespace rxcxx::extra::specific {
    template<class T>
    class string_evt : public evt_set<std::string, T> {
    };
}
#endif //STRING_EVT_HPP
