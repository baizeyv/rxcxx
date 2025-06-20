//
// Created by baizeyv on 6/20/2025.
//

#ifndef SELECT_H
#define SELECT_H
#include <functional>

#include "../base/abs_observer.hpp"

// ! safe completed

template<class T>
class abs_observable;

template<class T, class TR>
class select_observer final : public abs_observer<T> {
private:
    std::unique_ptr<abs_observer<TR> > observer;
    std::function<TR(T &)> selector;

public:
    select_observer(std::unique_ptr<abs_observer<TR> > observer,
                    std::function<TR(T &)> selector) : observer(std::move(observer)), selector(selector) {
    }

protected:
    void on_complete_core(std::unique_ptr<result> rst) override {
        observer->on_complete(std::move(rst));
    }

    void on_next_core(T &p_value) override {
        auto val = selector(p_value);
        observer->on_next(val);
    }

    void on_error_core(std::runtime_error &error) override {
        observer->on_error(error);
    }
};

template<class T, class TR>
class select final : public abs_observable<TR> {
private:
    std::unique_ptr<abs_observable<T> > source;
    std::function<TR(T &)> selector;

public:
    select(std::unique_ptr<abs_observable<T> > source, std::function<TR(T &)> selector) : source(std::move(source)),
        selector(selector) {
    }

protected:
    std::unique_ptr<disposable> subscribe_core(std::unique_ptr<abs_observer<TR> > observer) override {
        auto ptr = std::make_unique<select_observer<T, TR> >(std::move(observer), selector);
        return source->subscribe(std::move(ptr));
    }
};

#endif //SELECT_H
