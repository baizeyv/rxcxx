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

    static std::function<std::unique_ptr<result>(std::unique_ptr<result>)> handle_unique_result;
};

#endif //STUBS_H
