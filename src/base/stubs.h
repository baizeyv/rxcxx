//
// Created by baizeyv on 6/10/2025.
//

#ifndef STUBS_H
#define STUBS_H
#include <functional>

#include "result.h"


class stubs {
public:
    static std::function<void(std::runtime_error &)> unhandled_exception;

    static std::function<void(result *)> handle_result;
};

#endif //STUBS_H
