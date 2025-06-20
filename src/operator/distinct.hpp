//
// Created by baizeyv on 6/19/2025.
//

#ifndef DISTINCT_H
#define DISTINCT_H
#include <unordered_set>

#include "../base/abs_observer.hpp"

template<class T>
class abs_observable;

/**
 * * 每个值只会触发一次
 * ! 若是自定义类型,需要特化hash以及重载== (不重载的话使用默认比较)
 * @tparam T
 */
template<class T>
class distinct final : public abs_observable<T> {
private:
    /**
     * * source 被观察者指针
     */
    abs_observable<T>* source;

    template<class M>
    class distinct_observer final : public abs_observer<M> {
    private:
        abs_observer<M>* observer;
        std::unordered_set<M> set;
    public:
        explicit distinct_observer(abs_observer<M>* observer) : observer(observer) {}
        ~distinct_observer() override {
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
            if (const auto res = set.insert(p_value); res.second) {
                observer->on_next(p_value);
            }
        }

        void on_error_core(std::runtime_error &error) override {
            observer->on_error(error);
        }
    };

public:
    explicit distinct(abs_observable<T>* source) : source(source) {}
protected:
    disposable* subscribe_core(abs_observer<T> *observer) override {
        auto ob = TN(distinct_observer<T>,observer);
        // auto ob = new distinct_observer<T>(observer, count);
        // ! 这里的ob是new出来的,需要在合适的时机delete
        abs_observer<T>* ptr = static_cast<abs_observer<T>*>(ob);
        return source->subscribe(ptr);
    }
};

#endif //DISTINCT_H
