//
// Created by baizeyv on 6/19/2025.
//

#ifndef OPERATOR_FACTORY_H
#define OPERATOR_FACTORY_H
#include "skip.hpp"
#include "skip_while.h"
#include "take.hpp"
#include "where.hpp"
#include "../utils.h"
#include "../base/abs_observable.hpp"

class operator_factory final {
private:
    operator_factory() = default;
    ~operator_factory() = default;
public:

    static operator_factory* instance() {
        static operator_factory ins;
        return &ins;
    }

    // # 禁止移动
    operator_factory(const operator_factory&) = delete;

    // # 禁止赋值
    operator_factory& operator=(const operator_factory&) = delete;

    template<typename T>
    static abs_observable<T> *make_skip(abs_observable<T> *observable, const int count) {
        if (count >= 0) {
            return tracked_new<skip<T>>(observable, count);
            // return new skip<T>(observable, count);
        }
        throw std::runtime_error("argument out of range -> count");
    }

    template<typename T>
    static abs_observable<T> *make_take(abs_observable<T> *observable, const int count) {
        if (count >= 0) {
            return tracked_new<take<T>>(observable, count);
            // return new take<T>(observable, count);
        }
        throw std::runtime_error("argument out of range -> count");
    }

    template<typename T>
    static abs_observable<T> *make_where(abs_observable<T> *observable, std::function<bool(T&)> func) {
        return tracked_new<where<T>>(observable, func);
    }

    template<typename T>
    static abs_observable<T> *make_skip_while(abs_observable<T> *observable, std::function<bool(T&)> func) {
        return tracked_new<skip_while<T>>(observable, func);
    }
};
#endif //OPERATOR_FACTORY_H
