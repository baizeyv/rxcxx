//
// Created by baizeyv on 6/20/2025.
//

#ifndef SCAN_H
#define SCAN_H
#include <functional>

#include "operator.h"
#include "../base/abs_observer.hpp"

template<class T>
class abs_observable;

template<class T>
class scan_observer final : public abs_observer<T> {
private:
    abs_observer<T>* observer;
    std::function<T(T&,T&)> accumulator;
    T state;
    bool has_value;

public:
    scan_observer(abs_observer<T>* observer, std::function<T(T&, T&)> func) : observer(observer), accumulator(func), has_value(false) {

    }

    ~scan_observer() override {
        if (observer->is_disposed)
            return;
        // delete observer;
        TD(observer);
    }

protected:
    void on_complete_core(result *rst) override {
        observer->on_complete(rst);
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
class scan final : public abs_observable<T>, public operator_ {
private:
    /**
     * * source 被观察者指针
     */
    abs_observable<T> *source;

    std::function<T(T&,T&)> predicate;

public:
    scan(abs_observable<T>* source, std::function<T(T&,T&)> predicate) : source(source), predicate(predicate) {

    }

protected:
    disposable *subscribe_core(abs_observer<T> *observer) override {
        auto ob = TN(scan_observer<T>, observer, predicate);
        // ! 这里的ob是new出来的,需要在合适的时机delete
        abs_observer<T> *ptr = static_cast<abs_observer<T> *>(ob);
        return source->subscribe(ptr);
    }

    void release_core() override {
        TD(this);
    }

};

#endif //SCAN_H
