//
// Created by baizeyv on 6/19/2025.
//

#ifndef DISTINCT_H
#define DISTINCT_H
#include <unordered_set>

#include "../base/abs_observer.hpp"

// ! safe completed

template<class T>
class abs_observable;

template<class T>
class distinct_observer final : public abs_observer<T> {
private:
    std::unique_ptr<abs_observer<T>> observer;
    std::unordered_set<T> set;

public:
    explicit distinct_observer(std::unique_ptr<abs_observer<T>> observer) : observer(std::move(observer)) {
    }

protected:
    void on_complete_core(std::unique_ptr<result> rst) override {
        observer->on_complete(std::move(rst));
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
    std::unique_ptr<abs_observable<T> > source;

public:
    explicit distinct(std::unique_ptr<abs_observable<T> > source) : source(std::move(source)) {
    }

protected:
    std::unique_ptr<disposable> subscribe_core(std::unique_ptr<abs_observer<T> > observer) override {
        auto ptr = std::make_unique<distinct_observer<T> >(std::move(observer));
        return source->subscribe(std::move(ptr));
    }
};

#endif //DISTINCT_H
