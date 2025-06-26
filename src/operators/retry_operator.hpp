//
// Created by baizeyv on 6/26/2025.
//

#ifndef RETRY_OPERATOR_HPP
#define RETRY_OPERATOR_HPP
#include "../observable.hpp"

namespace rxcxx::operators {
    namespace retry_internal {
        template<typename Item>
        void do_retry(
            stream_controller<Item> sctl,
            observable<Item> source,
            const std::size_t max_retry_count,
            const std::size_t count
        ) noexcept {
            source.subscribe(sctl.template make_observer<Item>(
                [sctl](auto, const Item &x) {
                    sctl.sink_next(x);
                },
                [sctl, source, max_retry_count, count](auto serial, std::exception_ptr err) {
                    sctl.upstream_abort_observe(serial);
                    if (max_retry_count == 0 || count < max_retry_count) {
                        do_retry(sctl, source, max_retry_count, count + 1);
                    } else {
                        sctl.sink_error(err);
                    }
                },
                [sctl](auto serial) {
                    sctl.sink_completed(serial);
                }
            ));
        }
    } /* namespace retry_internal */

    inline auto retry(std::size_t max_retry_count = 0 /* infinite */) {
        return [max_retry_count](auto source) {
            using Source = decltype(source);
            using Item = typename Source::value_type;

            return observable<>::make_observable<Item>([source, max_retry_count](observer<Item> s) {
                auto sctl = stream_controller<Item>(s);
                retry_internal::do_retry<Item>(sctl, source, max_retry_count, 0);
            });
        };
    }
}

#endif //RETRY_OPERATOR_HPP
