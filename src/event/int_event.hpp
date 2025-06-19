//
// Created by baizeyv on 6/18/2025.
//

#ifndef INT_EVENT_H
#define INT_EVENT_H
#include <unordered_map>

#include "basic_event.hpp"
#include "simple_message.h"
#include "../base/disposable.h"


template<class T>
class int_event : public basic_event<int, T> {
};



#endif //INT_EVENT_H
