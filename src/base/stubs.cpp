//
// Created by baizeyv on 6/19/2025.
//

#include "stubs.h"

#include "../utils.h"

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
    // delete rst;
    TD(rst);
};

std::function<std::unique_ptr<result>(std::unique_ptr<result>)> stubs::handle_unique_result = [
        ](std::unique_ptr<result> rst) -> std::unique_ptr<result> {
    if (!rst)
        return rst;
    if (!rst->is_success) {
        auto ex = rst->get_exception();
        unhandled_exception(ex);
    }
    return std::move(rst);
};
