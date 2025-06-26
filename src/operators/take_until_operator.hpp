//
// Created by baizeyv on 6/26/2025.
//

#ifndef TAKE_UNTIL_OPERATOR_HPP
#define TAKE_UNTIL_OPERATOR_HPP
#include "../observable.hpp"

namespace rxcxx::operators {
    /**
     * 直到trigger发出任何信号,订阅结束
     * @tparam TRIGGER_OB
     * @param trigger
     * @return
     */
    template<typename TRIGGER_OB>
    auto take_until(TRIGGER_OB trigger) noexcept {
        // # TRIGGER_OB 是 observer
        return [trigger](auto source) {
            using Source = decltype(source); // # observable
            using Item = typename Source::value_type; // # observable的T
            using TriggerItem = typename TRIGGER_OB::value_type; // # observer的T
            return observable<>::make_observable<Item>(
                [source, trigger](observer<Item> ob) {
                    auto sctl = stream_controller<Item>(ob);

                    auto obs_trigger = sctl.template make_observer<TriggerItem>(
                        [sctl](auto, auto) {
                            sctl.sink_completed_force();
                        },
                        [sctl](auto, std::exception_ptr err) {
                            sctl.sink_error(err);
                        },
                        [sctl](auto serial) {
                            sctl.sink_completed(serial);
                        }
                    );

                    auto obs_source = sctl.template make_observer<Item>(
                        [sctl](auto, const Item &x) {
                            sctl.sink_next(x);
                        },
                        [sctl](auto, std::exception_ptr err) {
                            sctl.sink_error(err);
                        },
                        [sctl](auto serial) {
                            sctl.sink_completed_force(); // # trigger also dispose
                        }
                    );

                    trigger.subscribe(obs_trigger);
                    source.subscribe(obs_source);
                }
            );
        };
    }
}

#endif //TAKE_UNTIL_OPERATOR_HPP
