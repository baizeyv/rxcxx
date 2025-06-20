﻿//
// Created by baizeyv on 6/19/2025.
//

#ifndef SKIP_H
#define SKIP_H
#include "../utils.h"

template<class T>
class abs_observable;

template<class T>
class skip_observer final : public abs_observer<T> {
private:
    abs_observer<T>* observer;
    int count;

public:
    skip_observer(abs_observer<T>* observer, const int count) : observer(observer), count(count) {
    }

    ~skip_observer() override {
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
        if (count > 0) {
            --count;
        }
        else
            observer->on_next(p_value);
    }

    void on_error_core(std::runtime_error &error) override {
        observer->on_error(error);
    }
};


/**
 * * 跳过几次之后开始生效
 * @tparam T
 */
template <class T>
class skip final : public abs_observable<T>, public operator_ {
private:
    /**
     * * source 被观察者指针
     */
    abs_observable<T>* source;

    /**
     * * 需要跳过的数量
     */
    int count;

public:
    skip(abs_observable<T>* source, const int count) : source(source), count(count) {

    }

protected:
    disposable* subscribe_core(abs_observer<T> *observer) override {
        auto ob = TN(skip_observer<T>, observer, count);
        // auto ob = new skip_observer<T>(observer, count);
        // ! 这里的ob是new出来的,需要在合适的时机delete
        abs_observer<T>* ptr = static_cast<abs_observer<T>*>(ob);
        return source->subscribe(ptr);
    }

    void release_core() override {
        TD(this);
    }
};
#endif //SKIP_H
