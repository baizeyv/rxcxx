//
// Created by baizeyv on 6/26/2025.
//

#ifndef TAKE_LAST_OPERATOR_HPP
#define TAKE_LAST_OPERATOR_HPP
#include <queue>

#include "../observable.hpp"

namespace rxcxx::operators {
    inline auto take_last(size_t n) noexcept {
        return [n](auto source) {
            using Source = decltype(source); // # observable
            using Item = typename Source::value_type; // # observable 的T
            return observable<>::make_observable<Item>(
                [source, n](observer<Item> ob) {
                    auto sctl = stream_controller<Item>(ob);
                    auto mtx = std::make_shared<std::mutex>();
                    auto queue = std::make_shared<std::queue<Item>>();
                    source.subscribe(sctl.template make_observer<Item>(
                        [n, mtx, queue](auto, const Item& x) {
                            std::lock_guard<std::mutex> lock(*mtx);
                            queue->push(x);
                            if (queue->size() > n) {
                                queue->pop();
                            }
                        },
                        [sctl](auto, std::exception_ptr err) {
                            sctl.sink_error(err);
                        },
                        [sctl, n, mtx, queue](auto serial) {
                            {
                                std::lock_guard<std::mutex> lock(*mtx);
                                while (!queue->empty()) {
                                    sctl.sink_next(queue->front());
                                    queue->pop();
                                }
                            }
                            sctl.sink_completed(serial);
                        }
                    ));
                }
            );
        };
    }
}

#endif //TAKE_LAST_OPERATOR_HPP
