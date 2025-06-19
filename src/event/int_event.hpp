//
// Created by baizeyv on 6/18/2025.
//

#ifndef INT_EVENT_H
#define INT_EVENT_H
#include <unordered_map>

#include "simple_message.h"
#include "../base/disposable.h"


template<class T>
class int_event : public disposable{
protected:
    std::unordered_map<int, simple_event<T>> events;

    disposable* subscribe_core(int intEvent, std::function<void(T&)> on_next) {
        const auto it = events.find(intEvent);
        if (it != events.end()) {
            // # 找到了
            return events[intEvent].subscribe(on_next);
        }
        events[intEvent] = simple_event<T>();
        return events[intEvent].subscribe(on_next);
    }

public:

    disposable* subscribe(const int intEvent, std::function<void(T&)> on_next) {
        // ! 这里的observer是new出来的,需要在合适的时机delete
        disposable* subscription = subscribe_core(intEvent, on_next);
        // ! 这里的 subscription 是new出来的,需要在合适的时候delete (在observe->source_subscription 的 dispose 中会delete,也就是observer在dispose的时候)
        return subscription;
    }

    bool has_subscriptions() {
        return events.size() > 0;
    }

    template<typename U>
    void fire(int intEvent, U&& data) {
        const auto it = events.find(intEvent);
        if (it != events.end()) {
            // # 找到了
            events[intEvent].fire(std::forward<U>(data));
        }
    }

    void dispose() override {
        events.clear();
        // # 这里会自动调用析构函数,析构函数中会调用dispose,delete对象
    }

    ~int_event() {
        dispose();
    }
};



#endif //INT_EVENT_H
