//
// Created by baizeyv on 6/26/2025.
//

#ifndef FINALLY_OPERATOR_HPP
#define FINALLY_OPERATOR_HPP
#include "../observable.hpp"

namespace rxcxx::operators {
    template<typename F>
    inline auto finally(F f) noexcept {
        return [f](auto source) {
            using Source = decltype(source);
            using Item = typename Source::value_type;
            return observable<>::make_observable<Item>(
                [source, f](observer<Item> ob) {
                    auto sctl = stream_controller<Item>(ob);

                    source.subscribe(sctl.template make_observer<Item>(
                        [sctl](auto, const Item &x) {
                            sctl.sink_next(x);
                        },
                        [sctl, f](auto, std::exception_ptr err) {
                            sctl.sink_error(err);
                            f();
                        },
                        [sctl, f](auto serial) {
                            sctl.sink_completed(serial);
                            f();
                        }
                    ));
                }
            );
        };
    }
}

#endif //FINALLY_OPERATOR_HPP
