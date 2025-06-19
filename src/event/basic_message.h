//
// Created by baizeyv on 6/19/2025.
//

#ifndef BASIC_MESSAGE_H
#define BASIC_MESSAGE_H
#include "basic_event.hpp"
#include "../base/unit.h"

template<class T>
class basic_message : public basic_event<T, unit>{

};

#endif //BASIC_MESSAGE_H
