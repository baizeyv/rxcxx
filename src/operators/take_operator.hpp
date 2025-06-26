//
// Created by baizeyv on 6/25/2025.
//

#ifndef TAKE_OPERATOR_HPP
#define TAKE_OPERATOR_HPP
#include "../observable.hpp"
#include "../stream_controller.hpp"

namespace rxcxx::operators {
    inline auto take(size_t n) noexcept {
        return [n](auto source) {
            using Source = decltype(source); // # observable
            using Item = typename Source::value_type; // # observable 的 T
            return observable<>::make_observable<Item>([source, n](observer<Item> ob) {
                auto sctl = stream_controller<Item>(ob);
                auto counter = std::make_shared<std::atomic_size_t>(1);
                source.subscribe(sctl.template make_observer<Item>(
                    [sctl, n, counter](auto serial, const Item &x) {
                        if (const auto now = counter->fetch_add(1); now == n) {
                            sctl.upstream_abort_observe(serial);
                            sctl.sink_next(x);
                            sctl.sink_completed(serial);
                        } else if (now < n) {
                            sctl.sink_next(x);
                        }
                    },
                    [sctl](auto, std::exception_ptr err) { sctl.sink_error(err); },
                    [sctl](auto serial) { sctl.sink_completed(serial); }));
            });
        };
    }
} // namespace rxcxx::operators

#endif // TAKE_OPERATOR_HPP
