//
// Created by baizeyv on 6/10/2025.
//

#ifndef ABS_OBSERVABLE_H
#define ABS_OBSERVABLE_H
#include "abs_observer.hpp"
#include "anonymous_observer.hpp"
#include "disposable.h"
#include "../operator/operator.h"
#include "../operator/operator_factory.h"

/**
 * * 抽象被观察者
 * @tparam T
 */
template<class T>
class abs_observable {
public:
    virtual ~abs_observable() {
        // ! 这里再释放是为了防止已经订阅的事件没有取消订阅,最终导致operator悬空指针的问题 (release中已经判断了释放标识了)
        for (size_t i = 0; i < operator_pointers.size(); ++i) {
            operator_pointers[i]->release();
            // delete operator_pointers[i];
        }
        operator_pointers.clear();
    }

    disposable *subscribe(abs_observer<T> *observer) {
        // ! 这里传入的observer指针是new出来的,需要在合适的时候delete
        try {
            disposable *subscription = subscribe_core(observer);
            // ! 这里的 subscription 是new出来的,需要在合适的时候delete (在observe->source_subscription 的 dispose 中会delete,也就是observer在dispose的时候)
            observer->source_subscription.set_disposable(subscription);
            if (const auto ptr = dynamic_cast<operator_*>(this); ptr != nullptr) {
                // # 自身是操作符
                observer->source_operator_func = [ptr]() {
                    ptr->release();
                };
            }
            return static_cast<disposable *>(observer);
        } catch (...) {
            observer->dispose();
            throw;
        }
    }

    disposable *subscribe(std::function<void(T &)> on_next) {
        // auto observer = new anonymous_observer<T>(on_next, stubs::unhandled_exception, stubs::handle_result);
        auto observer = TN(anonymous_observer<T>, on_next, stubs::unhandled_exception, stubs::handle_result);
        // ! 这里的observer是new出来的,需要在合适的时机delete
        abs_observer<T> *ptr = static_cast<abs_observer<T> *>(observer);
        return this->subscribe(ptr);
    }

    abs_observable<T> *skip(const int count) {
        const auto pointer = operator_factory::make_skip<T>(this, count);
        operator_pointers.push_back(static_cast<operator_*>(pointer));
        return pointer;
    }

    abs_observable<T> *take(const int count) {
        const auto pointer = operator_factory::make_take<T>(this, count);
        operator_pointers.push_back(static_cast<operator_*>(pointer));
        return pointer;
    }

    abs_observable<T> *where(const std::function<bool(T &)> func) {
        const auto pointer = operator_factory::make_where<T>(this, func);
        operator_pointers.push_back(static_cast<operator_*>(pointer));
        return pointer;
    }

    abs_observable<T> *skip_while(const std::function<bool(T &)> func) {
        const auto pointer = operator_factory::make_skip_while<T>(this, func);
        operator_pointers.push_back(static_cast<operator_*>(pointer));
        return pointer;
    }

    abs_observable<T> *take_while(const std::function<bool(T &)> func) {
        const auto pointer = operator_factory::make_take_while<T>(this, func);
        operator_pointers.push_back(static_cast<operator_*>(pointer));
        return pointer;
    }

    abs_observable<T> *distinct() {
        const auto pointer = operator_factory::make_distinct<T>(this);
        operator_pointers.push_back(static_cast<operator_*>(pointer));
        return pointer;
    }

    abs_observable<T> *doo(std::function<void(T &)> on_next, const std::function<void(std::runtime_error &)> &on_error,
                           const std::function<void(result *)> &on_complete) {
        const auto pointer = operator_factory::make_do<T>(this, on_next, on_error, on_complete);
        operator_pointers.push_back(static_cast<operator_*>(pointer));
        return pointer;
    }

    abs_observable<T> *doo(std::function<void(T &)> on_next, const std::function<void(result *)> &on_complete) {
        const auto pointer = operator_factory::make_do<T>(this, on_next, on_complete);
        operator_pointers.push_back(static_cast<operator_*>(pointer));
        return pointer;
    }

    abs_observable<T> *doo(std::function<void(T &)> on_next) {
        const auto pointer = operator_factory::make_do<T>(this, on_next);
        operator_pointers.push_back(static_cast<operator_*>(pointer));
        return pointer;
    }

    template<typename TR>
    abs_observable<TR> *select(std::function<TR(T &)> func) {
        const auto pointer = operator_factory::make_select<T, TR>(this, func);
        operator_pointers.push_back(static_cast<operator_*>(pointer));
        return pointer;
    }

protected:
    std::vector<operator_ *> operator_pointers;

    virtual disposable *subscribe_core(abs_observer<T> *observer) = 0;
};
#endif //ABS_OBSERVABLE_H
