//
// Created by baizeyv on 6/27/2025.
//

#ifndef TIMEOUT_OPERATOR_HPP
#define TIMEOUT_OPERATOR_HPP
#include <chrono>
#include <condition_variable>
#include <stdexcept>
#include <thread>

#include "../observable.hpp"

namespace rxcxx::operators {
    class timeout_error : public std::runtime_error {
    public:
        explicit timeout_error(const std::string &msg) : runtime_error(msg) {
        }
    };

    inline auto timeout(std::chrono::milliseconds msec) noexcept {
        return [msec](auto source) {
            using Source = decltype(source);
            using Item = typename Source::value_type;
            return observable<>::make_observable<Item>([source, msec](observer<Item> s) {
                auto sctl = stream_controller<Item>(s);

                struct member {
                    std::mutex mtx_;
                    std::condition_variable cond_;
                };
                auto m = std::make_shared<member>();

                std::thread([sctl, m, msec] {
                    std::unique_lock<std::mutex> lock(m->mtx_);
                    while (sctl.is_subscribed()) {
                        if (m->cond_.wait_for(lock, msec) == std::cv_status::timeout) {
                            if (sctl.is_subscribed()) {
                                sctl.sink_error(std::make_exception_ptr(timeout_error("timeout")));
                            }
                            break;
                        }
                    }
                }).detach();

                source.subscribe(sctl.template make_observer<Item>(
                    [sctl, m](auto, const Item &x) {
                        m->cond_.notify_one();
                        sctl.sink_next(x);
                    },
                    [sctl, m](auto, std::exception_ptr err) {
                        sctl.sink_error(err);
                        m->cond_.notify_one();
                    },
                    [sctl, m](auto serial) {
                        sctl.sink_completed(serial);
                        m->cond_.notify_one();
                    }
                ));
            });
        };
    }
}

#endif //TIMEOUT_OPERATOR_HPP
