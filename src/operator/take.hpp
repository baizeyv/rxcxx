//
// Created by baizeyv on 6/19/2025.
//

#ifndef TAKE_H
#define TAKE_H
#include "../utils.h"
#include "../base/abs_observer.hpp"

template<class T>
class abs_observable;

/**
 * * 前几次生效
 * @tparam T
 */
template<class T>
class take final : public abs_observable<T> {
private:
    /**
     * * source 被观察者指针
     */
    abs_observable<T> *source;

    /**
     * * 生效次数
     */
    int count;

    template<class M>
    class take_observer final : public abs_observer<M> {
    private:
        abs_observer<M>* observer;
        int count;

    public:
        take_observer(abs_observer<M>* observer, const int count) : observer(observer), count(count) {
        }

        ~take_observer() override {
            if (observer->is_disposed) {
                return;
            }
            TD(observer);
        }

    protected:

        void on_complete_core(result *rst) override {
            observer->on_complete(rst);
            observer = nullptr;
        }

        void on_next_core(M &p_value) override {
            if (count <= 0)
                return;
            --count;
            observer->on_next(p_value);
            if (count != 0)
                return;
            observer->on_complete(result::Success());
        }

        void on_error_core(std::runtime_error &error) override {
            observer->on_error(error);
        }
    };
public:
    take(abs_observable<T>* source, const int count) : source(source), count(count) {
    }
protected:
    disposable* subscribe_core(abs_observer<T> *observer) override {
        auto ob = tracked_new<take_observer<T>>(observer, count);
        // auto ob = new take_observer<T>(observer, count);
        // ! 这里的ob是new出来的,需要在合适的时机delete
        abs_observer<T>* ptr = static_cast<abs_observer<T>*>(ob);
        return source->subscribe(ptr);
    }
};

#endif //TAKE_H
