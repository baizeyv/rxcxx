//
// Created by baizeyv on 6/26/2025.
//

#ifndef SKIP_OPERATOR_HPP
#define SKIP_OPERATOR_HPP
#include "../observable.hpp"

namespace rxcxx::operators {
    inline auto skip(size_t n) noexcept {
        return [n](auto source) {
            using Source = decltype(source); // # observable
            using Item = typename Source::value_type; // # observable的T
            return observable<>::make_observable<Item>(
                [source, n](observer<Item> ob) {
                    auto sctl = stream_controller<Item>(ob);
                    auto counter = std::make_shared<std::atomic_size_t>(1);
                    source.subscribe(sctl.template make_observer<Item>(
                            [sctl, n, counter](auto serial, const Item &x) {
                                const auto now = counter->fetch_add(1);
                                if (now > n) {
                                    sctl.sink_next(x);
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
                });
        };
    }
}

#endif //SKIP_OPERATOR_HPP
