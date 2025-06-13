//
// Created by baizeyv on 6/10/2025.
//

#ifndef STUBS_H
#define STUBS_H
#include <functional>

#include "result.h"


class stubs {

public:
    static std::function<void(std::runtime_error&)> unhandled_exception = [](const std::runtime_error &ex) {
        // TODO: GODOT LOG
        std::cout << "RX UnhandledException: " << ex.what() << std::endl;
    };

    static std::function<void(result*)> handle_result = [](const result* rst) {
        if (rst == nullptr)
            return;
        if (!rst->is_success) {
            auto ex = rst->get_exception();
            unhandled_exception(ex);
        }
        delete rst;
    };

};



#endif //STUBS_H
