//
// Created by baizeyv on 6/11/2025.
//

#ifndef REACTIVE_VARIABLE_H
#define REACTIVE_VARIABLE_H
#include "readonly_reactive_variable.hpp"
#include "subject.hpp"

template<class T>
class reactive_variable<T> : readonly_reactive_variable<T>, subject<T> {
private:
    T current;

    void throw_if_disposed() const {
        if (is_disposed()) {
            throw std::runtime_error("current reactive_variable has been disposed!");
        }
    }
public:
    bool is_disposed() const {
        // TODO:
    }

    reactive_variable<T>(T&& value) {
        // TODO:
    }

    reactive_variable<T>(T&& value, bool subscribe_with_init) {
        // TODO:
    }

    reactive_variable<T>(T&& value, bool call_on_value_changing_in_constructor, bool subscribe_with_init) {
        // TODO:
    }

    virtual T& value() {
        return current;
    }

    /**
     * * 重载赋值操作
     * @param v
     * @return
     */
    reactive_variable& operator=(const T& v) {
        on_value_changing(v);
        if (current == v)
            return *this;
        current = v;
        on_value_changed(v);
        on_next_core(v);
        return *this;
    }

    void dispose() override {
        // TODO:
    }

    void on_next(T& value) override {
        // TODO:
    }

    void on_error(std::runtime_error &error) override {
        // TODO:
    }

    void on_complete(result* rst) override {
        // TODO:
    }

    void dispose(bool call_on_completed) {
        // TODO:
    }
protected:
    disposable* subscribe_core(abs_observer<T>* observer) override {
        // TODO:
    }

    virtual void on_value_changing(T& value){}

    virtual void on_value_changed(T &value) override {
    }

    virtual void on_next_core(T& value) {
        // TODO:
    }
    virtual void dispose_core() {

    }
    // TODO:

};

#endif //REACTIVE_VARIABLE_H
