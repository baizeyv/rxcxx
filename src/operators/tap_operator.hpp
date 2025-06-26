//
// Created by baizeyv on 6/26/2025.
//

#ifndef TAP_OPERATOR_HPP
#define TAP_OPERATOR_HPP
#include "../observable.hpp"
#include "../observer.hpp"

namespace rxcxx::operators {
    template<typename T>
    auto tap(observer<T> obs) noexcept {
        return [obs](auto source) {
            using Source = decltype(source);
            using Item = typename Source::value_type;
            return observable<>::make_observable<Item>([source, obs](observer<Item> s) {
                auto sctl = stream_controller<Item>(s);
                source.subscribe(sctl.template make_observer<Item>(
                    [sctl, obs](auto, const Item &x) {
                        obs.on_next(x);
                        sctl.sink_next(x);
                    },
                    [sctl, obs](auto, std::exception_ptr err) {
                        obs.on_error(err);
                        sctl.sink_error(err);
                    },
                    [sctl, obs](auto serial) {
                        obs.on_completed();
                        sctl.sink_completed(serial);
                    }
                ));
            });
        };
    }

    template<typename ON_NEXT>
    auto tap(
        ON_NEXT n,
        observer<>::error_type e = {},
        observer<>::completed_type c = {}
    ) noexcept {
        return [n, e, c](auto source) {
            using Source = decltype(source);
            using Item = typename Source::value_type;
            return observable<>::make_observable<Item>([source, n, e, c](observer<Item> s) {
                auto sctl = stream_controller<Item>(s);
                source.subscribe(sctl.template make_observer<Item>(
                    [sctl, n](auto, const Item &x) {
                        n(x);
                        sctl.sink_next(x);
                    },
                    [sctl, e](auto, std::exception_ptr err) {
                        if (e) e(err);
                        sctl.sink_error(err);
                    },
                    [sctl, c](auto serial) {
                        if (c) c();
                        sctl.sink_completed(serial);
                    }
                ));
            });
        };
    }
}
#endif //TAP_OPERATOR_HPP
