//
// Created by baizeyv on 6/18/2025.
//

#ifndef SIMPLE_EVENT_H
#define SIMPLE_EVENT_H
#include "../base/abs_observable.hpp"
#include "../base/empty_disposable.h"
#include "../base/observer_node_parent.hpp"
#include "../base/subject.hpp"

template<class T>
class simple_event : public abs_observable<T>, public disposable, protected subject<T>, public observer_node_parent<T>{

private:
    T current;

    // # 1:success 2:failure 0:uncompleted 3: disposed
    unsigned int complete_state: 2;

    std::runtime_error error;

    void throw_if_disposed() const {
        if (is_disposed()) {
            throw std::runtime_error("current simple_event has been disposed!");
        }
    }

public:
    template<typename U>
    explicit simple_event(U && value) : complete_state(0), error(std::runtime_error("")) {
        current = std::forward<U>(value);
    }

    simple_event() : complete_state(0), error(std::runtime_error("")) {
        current = T();
    }

    ~simple_event() override {
        dispose(false);
    }

    /**
     * * 判断是否存在观察者
     * @return
     */
    bool has_observers() {
        return this->root != nullptr;
    }

    bool is_completed() const {
        return complete_state == 1 || complete_state == 2;
    }

    bool is_disposed() const {
        return complete_state == 3;
    }

    bool is_completed_or_disposed() const override {
        return is_completed() || is_disposed();
    }

    // # 可接收左值引用和右值引用
    template<typename U>
    void fire(U &&val) {
        on_next(val);
    }

    void fire() {
        this->on_next(current);
    }

    void on_next(T &value) override {
        throw_if_disposed();
        if (is_completed())
            return;
        observer_node<T>* node = this->root;
        observer_node<T>* last = nullptr;
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

    void on_error(std::runtime_error &error) override {
        throw_if_disposed();
        if (is_completed())
            return;
        observer_node<T>* node = this->root;
        observer_node<T>* last = nullptr;
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

    void on_complete(result* rst) override {
        throw_if_disposed();
        if (is_completed())
            return;
        observer_node<T>* node = nullptr;
        if (complete_state == 0) { // # not completed
            complete_state = rst->is_success ? 1 : 2;
            error = rst->get_exception();
            node = this->root;
            this->root = nullptr;
        } else {
            throw_if_disposed();
            return;
        }
        observer_node<T>* last = nullptr;
        if (node != nullptr) {
            last = node->previous;
        }
        while (node != nullptr) {
            node->observer->on_complete(rst);
            if (node == last)
                return;
            auto tmp = node;
            node = node->next;
            TD(tmp);
            // delete tmp;
        }
    }

    void dispose() override {
        dispose(true);
    }

    void dispose(const bool call_on_completed) {
        observer_node<T> *node = nullptr;
        if (complete_state == 3) // # disposed
        {
            return;
        }
        if (!is_completed()) {
            node = this->root;
        }
        this->root = nullptr;
        complete_state = 3;
        while (node) {
            // # 因为调用on_complete时会把node给delete掉,所以在delete前先存储上node->next
            auto next_ptr = node->next;
            if (call_on_completed)
                node->observer->on_complete(result::Success());
            else
                node->observer->on_complete_without_result();
            node = next_ptr;
        }
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
                observer->on_next(current);
            }
            observer->on_complete(completed_result);
            return TN(empty_disposable);
            // return new empty_disposable();
        }
        return TN(observer_node<T>, this, observer);
        // return new observer_node<T>(this, observer);
    }
};

#endif //SIMPLE_EVENT_H
