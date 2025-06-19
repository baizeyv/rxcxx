//
// Created by baizeyv on 6/19/2025.
//

#ifndef WHERE_H
#define WHERE_H
#include "../utils.h"
#include <functional>

#include "../base/abs_observer.hpp"
#include "../base/disposable.h"

template<class T>
class abs_observable;

template<class T>
class where final : public abs_observable<T> {
private:
    /**
     * * source 被观察者指针
     */
    abs_observable<T>* source;

    std::function<bool(T&)> predicate;

    template<class M>
    class where_observer final : public abs_observer<M> {
    private:
        abs_observer<M>* observer;
        std::function<bool(T&)> predicate;
    public:
        where_observer(abs_observer<M>* observer, std::function<bool(T&)> predicate) : observer(observer), predicate(predicate) {
        }

        ~where_observer() override {
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
            if (!predicate(p_value))
                return;
            observer->on_next(p_value);
        }

        void on_error_core(std::runtime_error &error) override {
            observer->on_error(error);
        }
    };

public:
    where(abs_observable<T>* source, std::function<bool(T&)> predicate) : source(source), predicate(predicate) {

    }

protected:
    disposable* subscribe_core(abs_observer<T> *observer) override {
        auto ob = tracked_new<where_observer<T>>(observer, predicate);
        // auto ob = new where_observer<T>(observer, predicate);
        // ! 这里的ob是new出来的,需要在合适的时机delete
        abs_observer<T>* ptr = static_cast<abs_observer<T>*>(ob);
        return source->subscribe(ptr);
    }
};

#endif //WHERE_H
