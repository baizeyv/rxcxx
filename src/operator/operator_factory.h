//
// Created by baizeyv on 6/19/2025.
//

#ifndef OPERATOR_FACTORY_H
#define OPERATOR_FACTORY_H
#include "skip.hpp"
#include "take.hpp"
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
};
#endif //OPERATOR_FACTORY_H
