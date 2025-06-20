﻿//
// Created by baizeyv on 6/10/2025.
//

#include "result.h"

#include <utility>

#include "../utils.h"

result::result() : exception(std::runtime_error("SUCCESS")), is_success(true) {
}

result::result(std::runtime_error exception) : exception(std::move(exception)), is_success(false) {
}

std::runtime_error& result::get_exception() {
    return exception;
}

result * result::Success() {
    return TN(result);
    // return new result();
}

result * result::Failure(std::runtime_error exception) {
    return TN(result, std::move(exception));
    // return new result(std::move(exception));
}

std::ostream &operator<<(std::ostream &os, result &r) {
    if (!r.is_success)
        os << "Failure {" << r.get_exception().what() << "}" << std::endl;
    else
        os << "Success" << std::endl;
    return os;
}
