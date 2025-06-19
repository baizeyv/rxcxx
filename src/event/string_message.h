//
// Created by baizeyv on 6/19/2025.
//

#ifndef STRING_MESSAGE_H
#define STRING_MESSAGE_H
#include "string_event.hpp"
#include "../base/unit.h"


class string_message final : public string_event<unit>{
public:
    void fire(const std::string &eventName);

};



#endif //STRING_MESSAGE_H
