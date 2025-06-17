//
// Created by baizeyv on 6/11/2025.
//

#ifndef READONLY_REACTIVE_VARIABLE_H
#define READONLY_REACTIVE_VARIABLE_H
#include "abs_observable.hpp"

template<class T>
class readonly_reactive_variable : public abs_observable<T>, public disposable {
public:
    virtual T &value() = 0;

    virtual void dispose() override = 0;

    readonly_reactive_variable<T> &to_readonly_reactive_variable() const { return &this; }

protected:
    virtual void on_value_changed(const T &value) {
    }

    virtual void on_receive_error(std::runtime_error &exception) {
    }
};

#endif //READONLY_REACTIVE_VARIABLE_H
