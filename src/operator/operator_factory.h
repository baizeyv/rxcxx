//
// Created by baizeyv on 6/19/2025.
//

#ifndef OPERATOR_FACTORY_H
#define OPERATOR_FACTORY_H
#include "distinct.h"
#include "skip.hpp"
#include "skip_while.h"
#include "take.hpp"
#include "take_while.h"
#include "where.hpp"
#include "do.h"
#include "../utils.h"

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
            return TN(skip<T>, observable, count);
            // return new skip<T>(observable, count);
        }
        throw std::runtime_error("argument out of range -> count");
    }

    template<typename T>
    static abs_observable<T> *make_take(abs_observable<T> *observable, const int count) {
        if (count >= 0) {
            return TN(take<T>, observable, count);
            // return new take<T>(observable, count);
        }
        throw std::runtime_error("argument out of range -> count");
    }

    template<typename T>
    static abs_observable<T> *make_where(abs_observable<T> *observable, std::function<bool(T&)> func) {
        return TN(where<T>, observable, func);
    }

    template<typename T>
    static abs_observable<T> *make_skip_while(abs_observable<T> *observable, std::function<bool(T&)> func) {
        return TN(skip_while<T>, observable, func);
    }

    template<typename T>
    static abs_observable<T> *make_take_while(abs_observable<T> *observable, std::function<bool(T&)> func) {
        return TN(take_while<T>, observable, func);
    }

    template<typename T>
    static abs_observable<T> *make_distinct(abs_observable<T> *observable) {
        return TN(distinct<T>, observable);
    }

    template<typename T>
    static abs_observable<T> *make_do(abs_observable<T> *observable, std::function<void(T &)> on_next, std::function<void(std::runtime_error &)> on_error, std::function<void(result *)> on_complete) {
        return TN(do_<T>, observable, on_next, on_error, on_complete);
    }

    template<typename T>
    static abs_observable<T> *make_do(abs_observable<T> *observable, std::function<void(T &)> on_next, std::function<void(result *)> on_complete) {
        return TN(do_<T>, observable, on_next, on_complete);
    }

    template<typename T>
    static abs_observable<T> *make_do(abs_observable<T> *observable, std::function<void(T &)> on_next) {
        return TN(do_<T>, observable, on_next);
    }
};
#endif //OPERATOR_FACTORY_H
