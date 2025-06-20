//
// Created by baizeyv on 6/20/2025.
//

#ifndef AGGREGATE_H
#define AGGREGATE_H
#include "operator.h"
#include "../base/abs_observer.hpp"

template<class T>
class abs_observable;

template<class T>
class aggregate_observer final : public abs_observer<T> {
private:
    abs_observer<T>* observer;
    std::function<T(T&,T&)> accumulator;
    T state;
    bool has_value;

public:
    aggregate_observer(abs_observer<T>* obs, std::function<T(T&,T&)> func) : observer(obs), accumulator(func), has_value(false) {}

    ~aggregate_observer() override {
        if (observer->is_disposed)
            return;
        // delete observer;
        TD(observer);
    }

protected:
    void on_complete_core(result *rst) override {
        if (!has_value) {
            observer->on_complete(rst);
            return;
        }
        observer->on_next(state);
        observer->on_complete(rst);
    }

    void on_next_core(T &p_value) override {
        if (!has_value) {
            has_value = true;
            state = p_value;
        } else {
            state = accumulator(state, p_value);
        }
    }

    void on_error_core(std::runtime_error &error) override {
        observer->on_error(error);
    }

    void dispose_core() override {

    }
};

template<class T>
class aggregate final : public abs_observable<T>, public operator_{
private:
    /**
     * * source 被观察者指针
     */
    abs_observable<T> *source;

    std::function<T(T&,T&)> predicate;
public:
    aggregate(abs_observable<T>* source, std::function<T(T&,T&)> predicate) : source(source), predicate(predicate) {

    }

protected:
    disposable *subscribe_core(abs_observer<T> *observer) override {
        auto ob = TN(aggregate_observer<T>, observer, predicate);
        // ! 这里的ob是new出来的,需要在合适的时机delete
        abs_observer<T> *ptr = static_cast<abs_observer<T> *>(ob);
        return source->subscribe(ptr);
    }

    void release_core() override {
        TD(this);
    }
};

#endif //AGGREGATE_H
