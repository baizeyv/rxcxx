//
// Created by baizeyv on 6/20/2025.
//

#ifndef DO_H
#define DO_H
#include <functional>
#include <stdexcept>
#include <utility>

#include "../base/abs_observer.hpp"
#include "../base/result.h"
#include "../base/stubs.h"

template<class T>
class abs_observable;

template<class T>
class do_observer final : public abs_observer<T> {
private:
    abs_observer<T>* observer;
    std::function<void(T &)> on_next;
    std::function<void(std::runtime_error &)> on_error;
    std::function<void(result *)> on_complete;
public:
    do_observer(abs_observer<T> *observer, std::function<void(T &)> on_next, std::function<void(std::runtime_error &)> on_error, std::function<void(result *)> on_complete) : observer(observer), on_next(on_next), on_error(std::move(on_error)), on_complete(std::move(on_complete)) {}
    ~do_observer() override {
        if (observer->is_disposed)
            return;
        // delete observer;
        TD(observer);
    }

protected:
    void on_complete_core(result *rst) override {
        on_complete(rst);
        observer->on_complete(rst);
    }

    void on_next_core(T &p_value) override {
        on_next(p_value);
        observer->on_next(p_value);
    }

    void on_error_core(std::runtime_error &error) override {
        on_error(error);
        observer->on_error(error);
    }
};

template<class T>
class do_ final : public abs_observable<T>, public operator_ {
private:
    /**
     * * source 被观察者指针
     */
    abs_observable<T>* source;

    std::function<void(T &)> on_next;
    std::function<void(std::runtime_error &)> on_error;
    std::function<void(result *)> on_complete;

public:
    do_(abs_observable<T>* source, std::function<void(T &)> on_next, std::function<void(std::runtime_error &)> on_error, std::function<void(result *)> on_complete) : source(source), on_next(on_next), on_error(std::move(on_error)), on_complete(std::move(on_complete)) {
    }

    do_(abs_observable<T>* source, std::function<void(T &)> on_next, std::function<void(result *)> on_complete) : source(source), on_next(on_next), on_error(stubs::unhandled_exception), on_complete(std::move(on_complete)) {
    }

    do_(abs_observable<T>* source, std::function<void(T &)> on_next) : source(source), on_next(on_next), on_error(stubs::unhandled_exception), on_complete(stubs::handle_result) {
    }
protected:
    disposable* subscribe_core(abs_observer<T> *observer) override {
        auto ob = TN(do_observer<T>, observer, on_next, on_error, on_complete);
        // auto ob = new do_observer<T>(observer, count);
        // ! 这里的ob是new出来的,需要在合适的时机delete
        abs_observer<T>* ptr = static_cast<abs_observer<T>*>(ob);
        return source->subscribe(ptr);
    }

    void release_core() override {
        TD(this);
    }
};

#endif //DO_H
