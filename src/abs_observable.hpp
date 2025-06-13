//
// Created by baizeyv on 6/10/2025.
//

#ifndef ABS_OBSERVABLE_H
#define ABS_OBSERVABLE_H
#include "abs_observer.hpp"
#include "disposable.h"

/**
 * * 抽象被观察者
 * @tparam T
 */
template<class T>
class abs_observable {
public:
    disposable* subscribe(abs_observer<T> *observer) {
        try {
            disposable* subscription = subscribe_core(observer);
            observer->source_subscription.set_disposable(subscription);
            return static_cast<disposable*>(observer);
        } catch (...) {
            observer->dispose();
            throw;
        }
    }
protected:
    virtual disposable* subscribe_core(abs_observer<T> *observer) = 0;
};
#endif //ABS_OBSERVABLE_H
