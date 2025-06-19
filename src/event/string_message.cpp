//
// Created by baizeyv on 6/19/2025.
//

#include "string_message.h"

void string_message::fire(const std::string &eventName) {
    const auto it = this->events.find(eventName);
    if (it != this->events.end()) {
        // # 找到了
        this->events[eventName].fire();
    }
}
