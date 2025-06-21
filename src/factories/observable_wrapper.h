//
// Created by baizeyv on 6/21/2025.
//

#ifndef OBSERVABLE_WRAPPER_H
#define OBSERVABLE_WRAPPER_H
#include <memory>

#include "../base/abs_observer.hpp"
#include "../base/disposable.h"

template<class T>
class abs_observable;

template<class T>
class observer_wrapper final : public abs_observer<T>
{
private:
    /**
     * * 观察者
     */
    abs_observer<T>* observer;

public:
    explicit observer_wrapper(abs_observer<T>* observer) : observer(observer) {}

protected:
    void on_complete_core(std::unique_ptr<result> rst) override
    {
        observer->on_complete(rst);
    }
    void on_next_core(T& p_value) override
    {
        observer->on_next(p_value);
    }
    void on_error_core(std::runtime_error& error) override
    {
        observer->on_error(error);
    }
};

template<class T>
class observable_wrapper final : public abs_observable<T>
{
private:
    /**
     * * 源被观察者
     */
    abs_observable<T>* source;

public:
    explicit observable_wrapper(abs_observable<T>* source) : source(source) {}

protected:
    std::unique_ptr<disposable> subscribe_core(std::unique_ptr<abs_observer<T>> observer) override {
        auto ptr = std::make_unique<observer_wrapper<T>>(std::move(observer));
        return source->subscribe(std::move(ptr));
    }
    
};

#endif //OBSERVABLE_WRAPPER_H
