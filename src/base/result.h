//
// Created by baizeyv on 6/10/2025.
//

#ifndef RESULT_H
#define RESULT_H
#include <stdexcept>
#include <iostream>
#include <memory>


struct result {
private:
    std::runtime_error exception;

public:
    bool is_success;

    result();

    explicit result(std::runtime_error exception);

    std::runtime_error& get_exception() ;

    static std::unique_ptr<result> Success();

    static std::unique_ptr<result> Failure(std::runtime_error exception);

};

std::ostream &operator<<(std::ostream &os, result &r);


#endif //RESULT_H
