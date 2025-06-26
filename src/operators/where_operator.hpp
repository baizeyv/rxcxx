//
// Created by baizeyv on 6/26/2025.
//

#ifndef WHERE_OPERATOR_HPP
#define WHERE_OPERATOR_HPP
#include "../observable.hpp"

namespace rxcxx::operators {
    template<typename F>
    inline auto where(F f) noexcept {
        return [f](auto source) {
            using Source = decltype(source); // # observable
            using Item = typename Source::value_type; // # observable的T
            return observable<>::make_observable<Item>(
                [source, f](observer<Item> ob) {
                    auto sctl = stream_controller<Item>(ob);
                    source.subscribe(sctl.template make_observer<Item>(
                        [sctl, f](auto, const Item &x) {
                            try {
                                if (f(x)) {
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
                    ));
                }
            );
        };
    }
}

#endif //WHERE_OPERATOR_HPP
