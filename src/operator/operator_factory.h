//
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
#include "../factories/observable_wrapper.h"

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
    static std::unique_ptr<observable_wrapper<T>> make_wrapper(abs_observable<T>* observable) {
        return std::make_unique<observable_wrapper<T>>(observable);
    }

    template<typename T>
    static std::unique_ptr<skip<T>> make_skip(std::unique_ptr<abs_observable<T>> observable, const int count) {
        if (count >= 0) {
            return std::make_unique<skip<T>>(std::move(observable), count);
        }
        throw std::runtime_error("argument out of range -> count");
    }

    template<typename T>
    static std::unique_ptr<abs_observable<T>> make_take(std::unique_ptr<abs_observable<T>> observable, const int count) {
        if (count >= 0) {
            return std::make_unique<take<T>>(std::move(observable), count);
        }
        throw std::runtime_error("argument out of range -> count");
    }

    template<typename T>
    static std::unique_ptr<where<T>> make_where(std::unique_ptr<abs_observable<T>> observable, std::function<bool(T&)> func) {
        return std::make_unique<where<T>>(std::move(observable), func);
    }

    template<typename T>
    static std::unique_ptr<scan<T>> make_scan(std::unique_ptr<abs_observable<T>> observable, std::function<T(T&,T&)> func) {
        return std::make_unique<scan<T>>(std::move(observable), func);
    }

    template<typename T>
    static std::unique_ptr<aggregate<T>> make_aggregate(std::unique_ptr<abs_observable<T>> observable, std::function<T(T&,T&)> func) {
        return std::make_unique<aggregate<T>>(std::move(observable), func);
    }

    template<typename T>
    static std::unique_ptr<skip_while<T>> make_skip_while(std::unique_ptr<abs_observable<T>> observable, std::function<bool(T&)> func) {
        return std::make_unique<skip_while<T>>(std::move(observable), func);
    }

    template<typename T>
    static std::unique_ptr<take_while<T>> make_take_while(std::unique_ptr<abs_observable<T>> observable, std::function<bool(T&)> func) {
        return std::make_unique<take_while<T>>(std::move(observable), func);
    }

    template<typename T>
    static std::unique_ptr<distinct<T>> make_distinct(std::unique_ptr<abs_observable<T>> observable) {
        return std::make_unique<distinct<T>>(std::move(observable));
    }

    template<typename T, typename TR>
    static std::unique_ptr<select<T, TR>> make_select(std::unique_ptr<abs_observable<T>> observable, std::function<TR(T&)> func) {
        return std::make_unique<select<T, TR>>(std::move(observable), func);
    }

    template<typename T>
    static std::unique_ptr<do_<T>> make_do(std::unique_ptr<abs_observable<T>> observable, std::function<void(T &)> on_next, std::function<void(std::runtime_error &)> on_error, std::function<void(std::unique_ptr<result>)> on_complete) {
        return std::make_unique<do_<T>>(std::move(observable), on_next, on_error, on_complete);
    }

    template<typename T>
    static std::unique_ptr<do_<T>> make_do(std::unique_ptr<abs_observable<T>> observable, std::function<void(T &)> on_next, std::function<void(std::unique_ptr<result>)> on_complete) {
        return std::make_unique<do_<T>>(std::move(observable), on_next, on_complete);
    }

    template<typename T>
    static std::unique_ptr<do_<T>> make_do(std::unique_ptr<abs_observable<T>> observable, std::function<void(T &)> on_next) {
        return std::make_unique<do_<T>>(std::move(observable), on_next);
    }
};
#endif //OPERATOR_FACTORY_H
