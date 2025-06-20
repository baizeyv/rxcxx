//
// Created by baizeyv on 6/19/2025.
//

#ifndef TAKE_H
#define TAKE_H
#include "../utils.h"
#include "../base/abs_observer.hpp"

// ! safe completed

template<class T>
class abs_observable;

template<class T>
class take_observer final : public abs_observer<T> {
private:
    std::unique_ptr<abs_observer<T>> observer;
    int count;

public:
    take_observer(std::unique_ptr<abs_observer<T>> observer, const int count) : observer(std::move(observer)), count(count) {
    }

protected:
    void on_complete_core(std::unique_ptr<result> rst) override {
        observer->on_complete(std::move(rst));
    }

    void on_next_core(T &p_value) override {
        if (count <= 0)
            return;
        --count;
        observer->on_next(p_value);
        if (count != 0)
            return;
        observer->on_complete(std::move(result::Success()));
    }

    void on_error_core(std::runtime_error &error) override {
        observer->on_error(error);
    }
};

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
    std::unique_ptr<abs_observable<T> > source;

    /**
     * * 生效次数
     */
    int count;

public:
    take(std::unique_ptr<abs_observable<T> > source, const int count) : source(std::move(source)), count(count) {
    }

protected:
    std::unique_ptr<disposable> subscribe_core(std::unique_ptr<abs_observer<T>> observer) override {
        auto ptr = std::make_unique<take_observer<T>>(std::move(observer), count);
        return source->subscribe(std::move(ptr));
    }
};

#endif //TAKE_H
