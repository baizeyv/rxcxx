//
// Created by baizeyv on 6/26/2025.
//

#ifndef SKIP_UNTIL_OPERATOR_HPP
#define SKIP_UNTIL_OPERATOR_HPP
#include "../observable.hpp"

namespace rxcxx::operators {
    template<typename TRIGGER_OB>
    auto skip_until(TRIGGER_OB trigger) noexcept {
        return [trigger](auto source) {
            using Source = decltype(source);
            using Item = typename Source::value_type;
            using TriggerItem = typename TRIGGER_OB::value_type;
            return observable<>::make_observable<Item>([source, trigger](observer<Item> s) {
                auto sctl = stream_controller<Item>(s);
                auto enable = std::make_shared<std::atomic_bool>(false);

                auto obs_trigger = sctl.template make_observer<TriggerItem>(
                    [sctl, enable](auto serial, auto) {
                        sctl.upstream_abort_observe(serial);
                        (*enable) = true;
                    },
                    [sctl](auto, std::exception_ptr err) {
                        sctl.sink_error(err);
                    },
                    [sctl](auto serial) {
                        sctl.sink_completed(serial);
                    }
                );

                auto obs_source = sctl.template make_observer<Item>(
                    [sctl, enable](auto, const Item &x) {
                        if (*enable) {
                            sctl.sink_next(x);
                        }
                    },
                    [sctl](auto, std::exception_ptr err) {
                        sctl.sink_error(err);
                    },
                    [sctl](auto serial) {
                        sctl.sink_completed_force(); // trigger also unsubscribe
                    }
                );

                trigger.subscribe(obs_trigger);
                source.subscribe(obs_source);
            });
        };
    }
}

#endif //SKIP_UNTIL_OPERATOR_HPP
