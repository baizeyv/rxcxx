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

std::function<void(std::runtime_error &)> stubs::unhandled_exception = [](const std::runtime_error &ex) {
    // TODO: GODOT LOG
    std::cout << "RX UnhandledException: " << ex.what() << std::endl;
};


std::function<void(result *)> stubs::handle_result = [](result *rst) {
    if (rst == nullptr)
        return;
    if (!rst->is_success) {
        auto ex = rst->get_exception();
        unhandled_exception(ex);
    }
    std::cout << "DELETE -> result " << rst << std::endl;
    delete rst;
};

#endif //STUBS_H
