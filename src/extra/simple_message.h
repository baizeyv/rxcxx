//
// Created by baizeyv on 6/24/2025.
//

#ifndef SIMPLE_MESSAGE_H
#define SIMPLE_MESSAGE_H
#include "simple_event.hpp"

namespace rxcxx {
    struct unit {
    };

    class simple_message : public simple_event<unit> {
    };
};


#endif //SIMPLE_MESSAGE_H
