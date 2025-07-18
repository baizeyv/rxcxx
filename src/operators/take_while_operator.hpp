﻿//
// Created by baizeyv on 6/26/2025.
//

#ifndef TAKE_WHILE_OPERATOR_HPP
#define TAKE_WHILE_OPERATOR_HPP
#include "../observable.hpp"

namespace rxcxx::operators {
    template<typename F>
    auto take_while(F f) noexcept {
        // # f 是一个 [](const Item& val) -> bool {} 的 lambda表达式
        return [f](auto source) {
            using Source = decltype(source); // # observable
            using Item = typename Source::value_type; //observable的T
            return observable<>::make_observable<Item>(
                [source, f](observer<Item> ob) {
                    auto sctl = stream_controller<Item>(ob);
                    source.subscribe(sctl.template make_observer<Item>(
                        [sctl, f](auto serial, const Item &x) {
                            try {
                                if (f(x)) {
                                    sctl.sink_next(x);
                                } else {
                                    sctl.sink_completed(serial);
                                }
                            } catch (...) {
                                sctl.sink_error(std::current_exception());
                            }
                        },
                        [sctl](auto, std::exception_ptr err) {
                            sctl.sink_error(err);
                        },
                        [sctl](auto serial) {
                            sctl.sink_completed(serial);
                        }
                    ));
                }
            );
        };
    };
}

#endif //TAKE_WHILE_OPERATOR_HPP
