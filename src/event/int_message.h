//
// Created by baizeyv on 6/19/2025.
//

#ifndef INT_MESSAGE_H
#define INT_MESSAGE_H
#include "int_event.hpp"

class int_message final : public int_event<unit>{
public:
    void fire(int intEvent);
};

#endif //INT_MESSAGE_H
