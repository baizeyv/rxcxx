//
// Created by baizeyv on 6/26/2025.
//

#ifndef ON_ERROR_RESUME_NEXT_OPERATOR_HPP
#define ON_ERROR_RESUME_NEXT_OPERATOR_HPP
#include "../observable.hpp"

namespace rxcxx::operators {
    template<typename NEXT_FN>
    inline auto on_error_resume_next(NEXT_FN f) noexcept {
        return [f](auto source) {
            using Source = decltype(source);
            using Item = typename Source::value_type;
            return observable<>::make_observable<Item>([source, f](observer<Item> s) {
                auto sctl = stream_controller<Item>(s);

                source.subscribe(sctl.template make_observer<Item>(
                    [sctl](auto, const Item &x) {
                        sctl.sink_next(x);
                    },
                    [sctl, f](auto serial, std::exception_ptr err) {
                        sctl.upstream_abort_observe(serial);
                        try {
                            f(err).subscribe(sctl.template make_observer<Item>(
                                [sctl](auto, const Item &x) {
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
                    [sctl](auto serial) {
                        sctl.sink_completed(serial);
                    }
                ));
            });
        };
    }
}

#endif //ON_ERROR_RESUME_NEXT_OPERATOR_HPP
