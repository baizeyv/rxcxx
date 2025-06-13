//
// Created by baizeyv on 6/10/2025.
//

#include "result.h"

#include <utility>

result::result() : exception(std::runtime_error("SUCCESS")), is_success(true) {
}

result::result(std::runtime_error exception) : exception(std::move(exception)), is_success(false) {
}

std::runtime_error result::get_exception() const {
    return exception;
}

result * result::Success() {
    return new result();
}

result * result::Failure(std::runtime_error exception) {
    return new result(std::move(exception));
}
