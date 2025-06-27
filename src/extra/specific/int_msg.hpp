//
// Created by baizeyv on 6/27/2025.
//

#ifndef INT_MSG_HPP
#define INT_MSG_HPP
#include <unordered_map>

#include "../msg_set.hpp"
#include "../rx_message.hpp"

namespace rxcxx::extra::specific {
    class int_msg : public msg_set<int> {
    };
}

#endif //INT_MSG_HPP
