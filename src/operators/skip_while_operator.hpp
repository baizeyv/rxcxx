//
// Created by baizeyv on 6/26/2025.
//

#ifndef SKIP_WHILE_OPERATOR_HPP
#define SKIP_WHILE_OPERATOR_HPP
#include "../observable.hpp"

namespace rxcxx::operators {
    template<typename F>
    inline auto skip_while(F f) noexcept {
        return [f](auto source) {
            using Source = decltype(source); // # observable
            using Item = typename Source::value_type; // # observable的T
            return observable<>::make_observable<Item>(
                [source,f](observer<Item> ob) {
                    auto sctl = stream_controller<Item>(ob);
                    auto skip = std::make_shared<bool>(true);
                    source.subscribe(
                        sctl.template make_observer<Item>(
                            [sctl, f, skip](auto, const Item &x) {
                                try {
                                    if (*skip) {
                                        *skip = f(x);
                                    }
                                    if (!*skip) {
                                        sctl.sink_next(x);
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
                        )
                    );
                }
            );
        };
    }
}

#endif //SKIP_WHILE_OPERATOR_HPP
