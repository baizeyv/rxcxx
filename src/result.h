//
// Created by baizeyv on 6/10/2025.
//

#ifndef RESULT_H
#define RESULT_H
#include <stdexcept>
#include <iostream>


struct result {
private:
    std::runtime_error exception;

public:
    bool is_success;

    result();

    explicit result(std::runtime_error exception);

    std::runtime_error get_exception() const;

    static result *Success();

    static result *Failure(std::runtime_error exception);

};

std::ostream &operator<<(std::ostream &os, const result &r) {
    if (!r.is_success)
        os << "Failure {" << r.get_exception().what() << "}" << std::endl;
    else
        os << "Success" << std::endl;
    return os;
}


#endif //RESULT_H
