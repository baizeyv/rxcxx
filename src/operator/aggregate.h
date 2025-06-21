//
// Created by baizeyv on 6/20/2025.
//

#ifndef AGGREGATE_H
#define AGGREGATE_H
#include "../base/abs_observer.hpp"

// ! safe completed

template<class T>
class abs_observable;

template<class T>
class aggregate_observer final : public abs_observer<T> {
private:
    std::unique_ptr<abs_observer<T> > observer;
    std::function<T(T &, T &)> accumulator;
    T state;
    bool has_value;

public:
    aggregate_observer(std::unique_ptr<abs_observer<T> > obs,
                       std::function<T(T &, T &)> func) : observer(std::move(obs)), accumulator(func),
                                                          has_value(false) {
    }

protected:
    void on_complete_core(std::unique_ptr<result> rst) override {
        if (!has_value) {
            observer->on_complete(std::move(rst));
            return;
        }
        observer->on_next(state);
        observer->on_complete(std::move(rst));
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
class aggregate final : public abs_observable<T> {
private:
    /**
     * * source 被观察者指针
     */
    std::unique_ptr<abs_observable<T> > source;

    std::function<T(T &, T &)> predicate;

public:
    aggregate(std::unique_ptr<abs_observable<T> > source,
              std::function<T(T &, T &)> predicate) : source(std::move(source)), predicate(predicate) {
    }

protected:
    std::unique_ptr<disposable> subscribe_core(std::unique_ptr<abs_observer<T> > observer) override {
        auto ptr = std::make_unique<aggregate_observer<T> >(std::move(observer), predicate);
        return source->subscribe(std::move(ptr));
    }
};

#endif //AGGREGATE_H
