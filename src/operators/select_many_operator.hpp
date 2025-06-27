//
// Created by baizeyv on 6/27/2025.
//

#ifndef SELECT_MANY_OPERATOR_HPP
#define SELECT_MANY_OPERATOR_HPP
#include "../observable.hpp"
#include "../rxcxx-utils.h"

namespace rxcxx::operators {
    template<typename F>
    auto flat_map(F f) noexcept {
        using OutObs = lambda_return_type_t<F>;
        using Out = typename OutObs::value_type;
        return [f](auto source) {
            return observable<>::make_observable<Out>([source, f](auto s) {
                using Source = decltype(source);
                using In = typename Source::value_type;

                auto sctl = stream_controller<Out>(s);

                source.subscribe(sctl.template make_observer<In>(
                    [sctl, f](auto, const In &x) {
                        try {
                            f(x).subscribe(sctl.template make_observer<Out>(
                                [sctl](auto, const Out &x) {
                                    sctl.sink_next(x);
                                },
                                [sctl](auto, std::exception_ptr err) {
                                    sctl.sink_error(err);
                                },
                                [sctl](auto serial) {
                                    sctl.sink_completed(serial);
                                }
                            ));
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
            });
        };
    }
}

#endif //SELECT_MANY_OPERATOR_HPP
