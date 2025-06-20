﻿//
// Created by baizeyv on 6/19/2025.
//

#ifndef OPERATOR_FACTORY_H
#define OPERATOR_FACTORY_H
#include "aggregate.h"
#include "distinct.hpp"
#include "skip.hpp"
#include "skip_while.hpp"
#include "take.hpp"
#include "take_while.hpp"
#include "where.hpp"
#include "do.hpp"
#include "scan.hpp"
#include "select.hpp"
#include "../utils.h"

class operator_factory final {
private:
    operator_factory() = default;
    ~operator_factory() = default;
public:

    // static operator_factory* instance() {
    //     static operator_factory ins;
    //     return &ins;
    // }

    // # 禁止移动
    operator_factory(const operator_factory&) = delete;

    // # 禁止赋值
    operator_factory& operator=(const operator_factory&) = delete;

    template<typename T>
    static skip<T> *make_skip(abs_observable<T> *observable, const int count) {
        if (count >= 0) {
            return TN(skip<T>, observable, count);
            // return new skip<T>(observable, count);
        }
        throw std::runtime_error("argument out of range -> count");
    }

    template<typename T>
    static take<T> *make_take(abs_observable<T> *observable, const int count) {
        if (count >= 0) {
            return TN(take<T>, observable, count);
            // return new take<T>(observable, count);
        }
        throw std::runtime_error("argument out of range -> count");
    }

    template<typename T>
    static where<T> *make_where(abs_observable<T> *observable, std::function<bool(T&)> func) {
        return TN(where<T>, observable, func);
    }

    template<typename T>
    static scan<T> *make_scan(abs_observable<T> *observable, std::function<T(T&,T&)> func) {
        return TN(scan<T>, observable, func);
    }

    template<typename T>
    static aggregate<T> *make_aggregate(abs_observable<T> *observable, std::function<T(T&,T&)> func) {
        return TN(aggregate<T>, observable, func);
    }

    template<typename T>
    static skip_while<T> *make_skip_while(abs_observable<T> *observable, std::function<bool(T&)> func) {
        return TN(skip_while<T>, observable, func);
    }

    template<typename T>
    static take_while<T> *make_take_while(abs_observable<T> *observable, std::function<bool(T&)> func) {
        return TN(take_while<T>, observable, func);
    }

    template<typename T>
    static distinct<T> *make_distinct(abs_observable<T> *observable) {
        return TN(distinct<T>, observable);
    }

    template<typename T, typename TR>
    static select<T, TR> *make_select(abs_observable<T> *observable, std::function<TR(T&)> func) {
        using myType = select<T, TR>;
        return TN(myType, observable, func);
    }

    template<typename T>
    static do_<T> *make_do(abs_observable<T> *observable, std::function<void(T &)> on_next, std::function<void(std::runtime_error &)> on_error, std::function<void(result *)> on_complete) {
        return TN(do_<T>, observable, on_next, on_error, on_complete);
    }

    template<typename T>
    static do_<T> *make_do(abs_observable<T> *observable, std::function<void(T &)> on_next, std::function<void(result *)> on_complete) {
        return TN(do_<T>, observable, on_next, on_complete);
    }

    template<typename T>
    static do_<T> *make_do(abs_observable<T> *observable, std::function<void(T &)> on_next) {
        return TN(do_<T>, observable, on_next);
    }
};
#endif //OPERATOR_FACTORY_H
