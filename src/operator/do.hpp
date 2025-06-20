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

// ! safe completed

template<class T>
class abs_observable;

template<class T>
class do_observer final : public abs_observer<T> {
private:
    std::unique_ptr<abs_observer<T> > observer;
    std::function<void(T &)> on_next;
    std::function<void(std::runtime_error &)> on_error;
    std::function<std::unique_ptr<result>(std::unique_ptr<result>)> on_complete;

public:
    do_observer(std::unique_ptr<abs_observer<T> > observer, std::function<void(T &)> on_next,
                std::function<void(std::runtime_error &)> on_error,
                std::function<std::unique_ptr<result>(std::unique_ptr<result>)> on_complete) : observer(std::move(
            observer)),
        on_next(on_next),
        on_error(std::move(on_error)),
        on_complete(std::move(on_complete)) {
    }

protected:
    void on_complete_core(std::unique_ptr<result> rst) override {
        auto ptr = on_complete(std::move(rst));
        observer->on_complete(std::move(ptr));
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
class do_ final : public abs_observable<T> {
private:
    /**
     * * source 被观察者指针
     */
    std::unique_ptr<abs_observable<T> > source;

    std::function<void(T &)> on_next;
    std::function<void(std::runtime_error &)> on_error;
    std::function<std::unique_ptr<result>(std::unique_ptr<result>)> on_complete;

public:
    do_(std::unique_ptr<abs_observable<T> > source, std::function<void(T &)> on_next,
        std::function<void(std::runtime_error &)> on_error,
        std::function<std::unique_ptr<result>(std::unique_ptr<result>)> on_complete) : source(std::move(source)),
        on_next(std::move(on_next)),
        on_error(std::move(on_error)),
        on_complete(std::move(on_complete)) {
    }

    do_(std::unique_ptr<abs_observable<T> > source, std::function<void(T &)> on_next,
        std::function<std::unique_ptr<result>(std::unique_ptr<result>)> on_complete) : source(std::move(source)),
        on_next(std::move(on_next)),
        on_error(stubs::unhandled_exception),
        on_complete(std::move(on_complete)) {
    }

    do_(std::unique_ptr<abs_observable<T> > source, std::function<void(T &)> on_next) : source(std::move(source)),
        on_next(on_next), on_error(stubs::unhandled_exception), on_complete(stubs::handle_unique_result) {
    }

protected:
    std::unique_ptr<disposable> subscribe_core(std::unique_ptr<abs_observer<T> > observer) override {
        auto ptr = std::make_unique<do_observer<T> >(std::move(observer), on_next, on_error, on_complete);
        return source->subscribe(std::move(ptr));
    }
};

#endif //DO_H
