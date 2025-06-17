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

    std::runtime_error& get_exception() ;

    static result *Success();

    static result *Failure(std::runtime_error exception);

};



#endif //RESULT_H
