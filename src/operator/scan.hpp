//
// Created by baizeyv on 6/20/2025.
//

#ifndef SCAN_H
#define SCAN_H
#include <functional>

#include "operator.h"
#include "../base/abs_observer.hpp"

// ! safe completed

template<class T>
class abs_observable;

template<class T>
class scan_observer final : public abs_observer<T> {
private:
    std::unique_ptr<abs_observer<T> > observer;
    std::function<T(T &, T &)> accumulator;
    T state;
    bool has_value;

public:
    scan_observer(std::unique_ptr<abs_observer<T> > observer,
                  std::function<T(T &, T &)> func) : observer(std::move(observer)), accumulator(func),
                                                     has_value(false) {
    }

protected:
    void on_complete_core(std::unique_ptr<result> rst) override {
        observer->on_complete(std::move(rst));
    }

    void on_next_core(T &p_value) override {
        if (!has_value) {
            has_value = true;
            state = p_value;
            observer->on_next(state);
            return;
        }
        state = accumulator(state, p_value);
        observer->on_next(state);
    }

    void on_error_core(std::runtime_error &error) override {
        observer->on_error(error);
    }
};

template<class T>
class scan final : public abs_observable<T> {
private:
    /**
     * * source 被观察者指针
     */
    std::unique_ptr<abs_observable<T> > source;

    std::function<T(T &, T &)> predicate;

public:
    scan(std::unique_ptr<abs_observable<T> > source, std::function<T(T &, T &)> predicate) : source(std::move(source)),
        predicate(predicate) {
    }

protected:
    std::unique_ptr<disposable> subscribe_core(std::unique_ptr<abs_observer<T> > observer) override {
        auto ptr = std::make_unique<scan_observer<T> >(std::move(observer), predicate);
        return source->subscribe(std::move(ptr));
    }
};

#endif //SCAN_H
