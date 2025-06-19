//
// Created by baizeyv on 6/10/2025.
//

#ifndef ABS_OBSERVABLE_H
#define ABS_OBSERVABLE_H
#include "abs_observer.hpp"
#include "anonymous_observer.hpp"
#include "disposable.h"
#include "../operator/operator_factory.h"

/**
 * * 抽象被观察者
 * @tparam T
 */
template<class T>
class abs_observable {
public:
    virtual ~abs_observable() {
        for (size_t i = 0; i < operator_pointers.size(); ++i) {
            TD(operator_pointers[i]);
            // delete operator_pointers[i];
        }
        operator_pointers.clear();
    }

    disposable* subscribe(abs_observer<T> *observer) {
        // ! 这里传入的observer指针是new出来的,需要在合适的时候delete
        try {
            disposable* subscription = subscribe_core(observer);
            // ! 这里的 subscription 是new出来的,需要在合适的时候delete (在observe->source_subscription 的 dispose 中会delete,也就是observer在dispose的时候)
            observer->source_subscription.set_disposable(subscription);
            return static_cast<disposable*>(observer);
        } catch (...) {
            observer->dispose();
            throw;
        }
    }

    disposable* subscribe(std::function<void(T&)> on_next) {
        // auto observer = new anonymous_observer<T>(on_next, stubs::unhandled_exception, stubs::handle_result);
        auto observer = tracked_new<anonymous_observer<T>>(on_next, stubs::unhandled_exception, stubs::handle_result);
        // ! 这里的observer是new出来的,需要在合适的时机delete
        abs_observer<T>* ptr = static_cast<abs_observer<T>*>(observer);
        return this->subscribe(ptr);
    }

    abs_observable<T>* skip(const int count) {
        const auto pointer = operator_factory::make_skip<T>(this, count);
        operator_pointers.push_back(pointer);
        return pointer;
    }

    abs_observable<T>* take(const int count) {
        const auto pointer = operator_factory::make_take<T>(this, count);
        operator_pointers.push_back(pointer);
        return pointer;
    }

    abs_observable<T>* where(const std::function<bool(T&)> func) {
        const auto pointer = operator_factory::make_where<T>(this, func);
        operator_pointers.push_back(pointer);
        return pointer;
    }

    abs_observable<T>* skip_while(const std::function<bool(T&)> func) {
        const auto pointer = operator_factory::make_skip_while<T>(this, func);
        operator_pointers.push_back(pointer);
        return pointer;
    }

    abs_observable<T>* take_while(const std::function<bool(T&)> func) {
        const auto pointer = operator_factory::make_take_while<T>(this, func);
        operator_pointers.push_back(pointer);
        return pointer;
    }

protected:

    std::vector<abs_observable<T>*> operator_pointers;

    virtual disposable* subscribe_core(abs_observer<T> *observer) = 0;
};
#endif //ABS_OBSERVABLE_H
