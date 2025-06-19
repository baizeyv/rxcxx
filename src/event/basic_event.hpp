//
// Created by baizeyv on 6/19/2025.
//

#ifndef BASIC_EVENT_H
#define BASIC_EVENT_H
#include <unordered_map>

#include "simple_event.hpp"
#include "../base/disposable.h"

/**
 * ! T是要在unordered_map中充当key的角色的,有需要的话可以重写比较方法 (特化hash以及重载==)
 * @tparam T Key
 * @tparam U Value Argument
 */
template<class T,class U>
class basic_event : public disposable {
protected:
    std::unordered_map<T, simple_event<U>> events;

    disposable* subscribe_core(T eventName, std::function<void(U&)> on_next) {
        const auto it = events.find(eventName);
        if (it != events.end()) {
            // # 找到了
            return events[eventName].subscribe(on_next);
        }
        events[eventName] = simple_event<U>();
        return events[eventName].subscribe(on_next);
    }

public:

    disposable* subscribe(const T eventName, std::function<void(U&)> on_next) {
        // ! 这里的observer是new出来的,需要在合适的时机delete
        disposable* subscription = subscribe_core(eventName, on_next);
        // ! 这里的 subscription 是new出来的,需要在合适的时候delete (在observe->source_subscription 的 dispose 中会delete,也就是observer在dispose的时候)
        return subscription;
    }

    bool has_subscriptions() {
        return !events.empty();
    }

    template<typename G>
    void fire(T eventName, G&& data) {
        const auto it = events.find(eventName);
        if (it != events.end()) {
            // # 找到了
            events[eventName].fire(std::forward<G>(data));
        }
    }

    void dispose() override {
        events.clear();
        // # 这里会自动调用析构函数,析构函数中会调用dispose,delete对象
    }

    ~basic_event() {
        basic_event<T, U>::dispose();
    }
};

#endif //BASIC_EVENT_H
