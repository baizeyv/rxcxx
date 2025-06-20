//
// Created by baizeyv on 6/10/2025.
//

#ifndef ABS_OBSERVABLE_H
#define ABS_OBSERVABLE_H
#include <memory>

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

    std::unique_ptr<disposable> subscribe(std::unique_ptr<abs_observer<T>> observer) {
        try {
            std::unique_ptr<disposable> subscription = subscribe_core(observer);
            observer->source_subscription.set_disposable(subscription);
            std::unique_ptr<disposable> ptr = std::move(observer);
            return ptr;
        } catch (...) {
            observer->dispose();
            throw;
        }
    }

    std::unique_ptr<disposable> subscribe(std::function<void(T &)> on_next) {
        auto ptr = std::make_unique<anonymous_observer<T>>(on_next, stubs::unhandled_exception, stubs::handle_result);
        return this->subscribe(std::move(ptr));
    }

    std::unique_ptr<abs_observable<T>> skip(const int count) {
        const auto pointer = operator_factory::make_skip<T>(this, count);
        return pointer;
    }

    abs_observable<T> *take(const int count) {
        const auto pointer = operator_factory::make_take<T>(this, count);
        return pointer;
    }

    abs_observable<T> *where(const std::function<bool(T &)> func) {
        const auto pointer = operator_factory::make_where<T>(this, func);
        return pointer;
    }

    abs_observable<T> *scan(const std::function<T(T&,T&)> func) {
        const auto pointer = operator_factory::make_scan<T>(this, func);
        return pointer;
    }

    abs_observable<T> *aggregate(const std::function<T(T&,T&)> func) {
        const auto pointer = operator_factory::make_aggregate<T>(this, func);
        return pointer;
    }

    abs_observable<T> *skip_while(const std::function<bool(T &)> func) {
        const auto pointer = operator_factory::make_skip_while<T>(this, func);
        return pointer;
    }

    abs_observable<T> *take_while(const std::function<bool(T &)> func) {
        const auto pointer = operator_factory::make_take_while<T>(this, func);
        return pointer;
    }

    abs_observable<T> *distinct() {
        const auto pointer = operator_factory::make_distinct<T>(this);
        return pointer;
    }

    abs_observable<T> *doo(std::function<void(T &)> on_next, const std::function<void(std::runtime_error &)> &on_error,
                           const std::function<void(result *)> &on_complete) {
        const auto pointer = operator_factory::make_do<T>(this, on_next, on_error, on_complete);
        return pointer;
    }

    abs_observable<T> *doo(std::function<void(T &)> on_next, const std::function<void(result *)> &on_complete) {
        const auto pointer = operator_factory::make_do<T>(this, on_next, on_complete);
        return pointer;
    }

    abs_observable<T> *doo(std::function<void(T &)> on_next) {
        const auto pointer = operator_factory::make_do<T>(this, on_next);
        return pointer;
    }

    template<typename TR>
    abs_observable<TR> *select(std::function<TR(T &)> func) {
        const auto pointer = operator_factory::make_select<T, TR>(this, func);
        return pointer;
    }

protected:

    virtual std::unique_ptr<disposable> subscribe_core(std::unique_ptr<abs_observer<T>> observer) = 0;
};
#endif //ABS_OBSERVABLE_H
