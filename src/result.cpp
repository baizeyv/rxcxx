//
// Created by baizeyv on 6/10/2025.
//

#include "result.h"

#include <utility>

result::result() : exception(std::runtime_error("SUCCESS")), is_success(true) {
}

result::result(std::runtime_error exception) : exception(std::move(exception)), is_success(false) {
}

std::runtime_error& result::get_exception() {
    return exception;
}

result * result::Success() {
    const auto tmp = new result();
    std::cout << "NEW -> result(success) " << tmp << std::endl;
    return tmp;
}

result * result::Failure(std::runtime_error exception) {
    const auto tmp = new result(std::move(exception));
    std::cout << "NEW -> result(failure) " << tmp << std::endl;
    return tmp;
}

std::ostream &operator<<(std::ostream &os, result &r) {
    if (!r.is_success)
        os << "Failure {" << r.get_exception().what() << "}" << std::endl;
    else
        os << "Success" << std::endl;
    return os;
}
