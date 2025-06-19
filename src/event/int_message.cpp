//
// Created by baizeyv on 6/19/2025.
//
#include "int_message.h"


void int_message::fire(const int intEvent) {
    const auto it = this->events.find(intEvent);
    if (it != this->events.end()) {
        // # 找到了
        this->events[intEvent].fire();
    }
}
