//
// Created by baizeyv on 6/10/2025.
//

#ifndef EMPTY_DISPOSABLE_H
#define EMPTY_DISPOSABLE_H
#include "disposable.h"

class empty_disposable final : public disposable {
public:
    void dispose() override {
        std::cout << "DELETE -> disposable(empty) " << this << std::endl;
        delete this;
    };
};
#endif //EMPTY_DISPOSABLE_H
