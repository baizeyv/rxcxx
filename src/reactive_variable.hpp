//
// Created by baizeyv on 6/11/2025.
//

#ifndef REACTIVE_VARIABLE_H
#define REACTIVE_VARIABLE_H

#include "empty_disposable.h"
#include "observer_node_parent.hpp"
#include "readonly_reactive_variable.hpp"
#include "subject.hpp"

// ! 若T为自定义类型,推荐重载==
template<class T>
class reactive_variable : public readonly_reactive_variable<T>, protected subject<T>, protected observer_node_parent<T> {
private:
    T current;

    bool subscribe_with_init;

    // # 1:success 2:failure 0:uncompleted 3: disposed
    unsigned int complete_state: 2;

    std::runtime_error error;

    void throw_if_disposed() const {
        if (is_disposed()) {
            throw std::runtime_error("current reactive_variable has been disposed!");
        }
    }

public:
    bool is_completed() const {
        return complete_state == 1 || complete_state == 2;
    }

    bool is_disposed() const {
        return complete_state == 3;
    }


    T &value() override {
        return current;
    }

    template<typename U>
    explicit reactive_variable(U &&value): complete_state(0), error(std::runtime_error("")) {
        subscribe_with_init = false;
        reactive_variable<T>::on_value_changing(value);
        current = std::forward<U>(value);
        reactive_variable<T>::on_value_changed(value);
    }

    reactive_variable(T &value, bool subscribe_with_init) : subscribe_with_init(subscribe_with_init), complete_state(0),
                                                            error(std::runtime_error("")) {
        reactive_variable<T>::on_value_changing(value);
        current = value;
        reactive_variable<T>::on_value_changed(value);
    }

    reactive_variable
    (T &value, const bool call_on_value_changing_in_constructor,
     const bool subscribe_with_init) : subscribe_with_init(subscribe_with_init),
                                       complete_state(0), error(std::runtime_error("")) {
        if (call_on_value_changing_in_constructor)
            reactive_variable<T>::on_value_changing(value);
        current = value;
        if (call_on_value_changing_in_constructor)
            reactive_variable<T>::on_value_changed(value);
    }

    /**
     * * 重载赋值操作
     * @param v
     * @return
     */
    reactive_variable &operator=(T&& v) {
        on_value_changing(v);
        if (current == v)
            return *this;
        current = v;
        on_value_changed(v);
        on_next_core(v);
        return *this;
    }

    void dispose() override {
        dispose(true);
    }


    bool is_completed_or_disposed() const override {
        return is_completed() || is_disposed();
    }

    void on_next(T &value) override {
        this->on_value_changing(value);
        current = value;
        this->on_value_changed(value);
        on_next_core(value);
    }

    void on_error(std::runtime_error &error) override {
        throw_if_disposed();
        if (is_completed())
            return;
        this->on_receive_error(error);
        observer_node<T> *node = this->root;
        observer_node<T> *last;
        if (node != nullptr) {
            last = node->previous;
        }
        while (node != nullptr) {
            node->observer->on_error(error);
            if (node == last)
                return;
            node = node->next;
        }
    }

    void on_complete(result *rst) override {
        throw_if_disposed();
        if (is_completed())
            return;
        observer_node<T> *node = nullptr;
        if (complete_state == 0) // # not completed
        {
            complete_state = rst->is_success ? 1 : 2;
            error = rst->get_exception();
            node = this->root;
            this->root = nullptr;
        } else {
            throw_if_disposed();
            return;
        }
        if (!rst->is_success) {
            this->on_receive_error(rst->get_exception());
        }
        observer_node<T> *last;
        if (node != nullptr) {
            last = node->previous;
        }
        while (node != nullptr) {
            node->observer->on_complete(rst);
            if (node == last)
                return;
            auto tmp = node;
            node = node->next;
            std::cout << "DELETE -> disposable(observer node) " << tmp << std::endl;
            delete tmp;
        }
    }

    void dispose(const bool call_on_completed) {
        // TODO: delete new 出的对象指针
        observer_node<T> *node = nullptr;
        if (complete_state == 3) // # disposed
        {
            return;
        }
        if (call_on_completed && !is_completed()) {
            node = this->root;
        }
        this->root = nullptr;
        complete_state = 3;
        while (node) {
            // # 因为调用on_complete时会把node给delete掉,所以在delete前先存储上node->next
            auto next_ptr = node->next;
            node->observer->on_complete(result::Success());
            node = next_ptr;
        }
        dispose_core();
    }

protected:
    disposable *subscribe_core(abs_observer<T> *observer) override {
        // ! 这里传入的observer指针是new出来的,需要在合适的时候delete
        throw_if_disposed();
        result *completed_result = nullptr;
        if (is_completed()) {
            completed_result = error.what() == "" ? result::Success() : result::Failure(error);
        }
        if (completed_result != nullptr) {
            if (completed_result->is_success) {
                if (subscribe_with_init)
                    observer->on_next(current);
            }
            observer->on_complete(completed_result);
            const auto tmp = new empty_disposable();;
            std::cout << "NEW -> disposable(empty) " << tmp << std::endl;
            return tmp;
        }

        if (subscribe_with_init)
            observer->on_next(current);

        const auto tmp = new observer_node<T>(this, observer);;
        std::cout << "NEW -> disposable(observer_node) " << tmp << std::endl;
        return tmp;
    }

    virtual void on_value_changing(T &value) {
    }

    void on_value_changed(const T &value) override {
    }

    virtual void on_next_core(T &value) {
        throw_if_disposed();
        if (is_completed())
            return;
        observer_node<T> *node = this->root;
        observer_node<T> *last;
        if (node != nullptr) {
            last = node->previous;
        }
        while (node != nullptr) {
            node->observer->on_next(value);
            if (node == last)
                return;
            node = node->next;
        }
    }

    virtual void dispose_core() {
    }
};

#endif //REACTIVE_VARIABLE_H
