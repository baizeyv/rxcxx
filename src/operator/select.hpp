//
// Created by baizeyv on 6/20/2025.
//

#ifndef SELECT_H
#define SELECT_H
#include <functional>

#include "../base/abs_observer.hpp"

template<class T>
class abs_observable;

template<class T, class TR>
class select_observer final : public abs_observer<T>{
private:
    abs_observer<TR>* observer;
    std::function<TR(T&)> selector;

public:
    select_observer(abs_observer<TR>* observer,
                    std::function<TR(T &)> selector) : observer(observer), selector(selector) {
    }

    ~select_observer() override {
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
        auto val = selector(p_value);
        observer->on_next(val);
    }

    void on_error_core(std::runtime_error &error) override {
        observer->on_error(error);
    }

};

template<class T, class TR>
class select final : public abs_observable<TR>, public operator_{
private:
    abs_observable<T> *source;
    std::function<TR(T &)> selector;

public:
    select(abs_observable<T> *source, std::function<TR(T &)> selector) : source(source), selector(selector) {
    }

protected:

    disposable *subscribe_core(abs_observer<TR> *observer) override {
        using myType = select_observer<T, TR>;
        auto ob = TN(myType, observer, selector);
        // ! 这里的ob是new出来的,需要在合适的时机delete
        abs_observer<T> *ptr = static_cast<abs_observer<T> *>(ob);
        return source->subscribe(ptr);
    }

    void release_core() override {
        TD(this);
    }
};

#endif //SELECT_H
