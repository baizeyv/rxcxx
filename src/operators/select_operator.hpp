//
// Created by baizeyv on 6/26/2025.
//

#ifndef SELECT_OPERATOR_HPP
#define SELECT_OPERATOR_HPP
#include "../observable.hpp"
#include "../rxcxx-utils.h"

namespace rxcxx::operators {
    template<typename F>
    inline auto select(F f) noexcept {
        return [f](auto source) {
            using Source = decltype(source); // # observable
            using In = typename Source::value_type; // # observable 的 T
            using Out = lambda_return_type_t<F, In>;
            return observable<>::make_observable<Out>(
                [source, f](auto ob) {
                    auto sctl = stream_controller<Out>(ob);
                    source.subscribe(
                        sctl.template make_observer<In>(
                            [sctl, f](auto, const In &x) {
                                try {
                                    sctl.sink_next(f(x));
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

#endif //SELECT_OPERATOR_HPP
