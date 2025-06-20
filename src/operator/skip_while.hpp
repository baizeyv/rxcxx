//
// Created by baizeyv on 6/19/2025.
//

#ifndef SKIP_WHILE_H
#define SKIP_WHILE_H
#include <functional>

#include "../base/abs_observer.hpp"

// ! safe completed

template<class T>
class abs_observable;

template<class T>
class skip_while_observer final : public abs_observer<T> {
private:
    std::unique_ptr<abs_observer<T> > observer;
    std::function<bool(T &)> predicate;
    bool open;

public:
    skip_while_observer(std::unique_ptr<abs_observer<T> > observer,
                        std::function<bool(T &)> predicate) : observer(std::move(observer)),
                                                              predicate(predicate), open(false) {
    }

protected:
    void on_complete_core(std::unique_ptr<result> rst) override {
        observer->on_complete(std::move(rst));
    }

    void on_next_core(T &p_value) override {
        if (open) {
            observer->on_next(p_value);
        } else if (!predicate(p_value)) {
            open = true;
            observer->on_next(p_value);
        }
    }

    void on_error_core(std::runtime_error &error) override {
        observer->on_error(error);
    }
};

template<class T>
class skip_while final : public abs_observable<T> {
private:
    /**
     * * source 被观察者指针
     */
    std::unique_ptr<abs_observable<T> > source;

    std::function<bool(T &)> predicate;

public:
    skip_while(std::unique_ptr<abs_observable<T> > source,
               std::function<bool(T &)> predicate) : source(std::move(source)), predicate(predicate) {
    }

protected:
    std::unique_ptr<disposable> subscribe_core(std::unique_ptr<abs_observer<T> > observer) override {
        auto ptr = std::make_unique<skip_while_observer<T> >(std::move(observer), predicate);
        return source->subscribe(std::move(ptr));
    }
};

#endif //SKIP_WHILE_H
