//
// Created by baizeyv on 6/26/2025.
//

#ifndef REPLAY_HPP
#define REPLAY_HPP
#include <queue>

#include "subject.hpp"

namespace rxcxx::subjects {
    template<class T>
    class replay : public subject<T> {
    private:
        struct member {
            std::shared_ptr<std::queue<T>> queue;
            size_t replay_times;
            std::mutex mtx;
            subscription subscription;

            explicit member(size_t replay_times) : queue(std::make_shared<std::queue<T>>()), replay_times(replay_times) {
            }

            ~member() {
                subscription.dispose();
            };
        };

        std::shared_ptr<member> mem;

        void initialize() {
            auto m = mem;
            m->subscription = this->as_observable().subscribe(
                {
                    [m](const T &x) {
                        std::lock_guard<std::mutex> lock(m->mtx);
                        if (m->queue->size() == m->replay_times) {
                            m->queue->pop();
                        }
                        m->queue->push(std::move(x));
                    },
                    [](std::exception_ptr _) {
                    },
                    []() {
                    }
                }
            );
        }

    public:
        explicit replay(size_t&& replay_times) noexcept : mem(std::make_shared<member>(std::move(replay_times))) {
            initialize();
        }

        explicit replay(const size_t& replay_times) noexcept : mem(std::make_shared<member>(replay_times)) {
            initialize();
        }

        ~replay() = default;

        observable<T> as_observable() const noexcept override {
            auto src = subject<T>::as_observable();
            auto m = mem;
            auto base_completed = subject<T>::completed();
            auto base_error = subject<T>::error();
            return observable<>::make_observable<T>(
                [src, m, base_completed, base_error](observer<T> ob) {
                    auto sctl = stream_controller<T>(ob);
                    if (base_completed) {
                        if (base_error != nullptr) {
                            sctl.sink_error(base_error);
                        } else {
                            sctl.sink_completed_force();
                        }
                    } else {
                        {
                            std::lock_guard<std::mutex> lock(m->mtx);
                            std::queue<T> q = *m->queue;
                            while (!q.empty()) {
                                sctl.sink_next(q.front());
                                q.pop();
                            }
                        }
                        src.subscribe(sctl.template make_observer<T>(
                            [sctl](auto, const T &x) {
                                sctl.sink_next(x);
                            },
                            [sctl](auto, std::exception_ptr err) {
                                sctl.sink_error(err);
                            },
                            [sctl](auto serial) {
                                sctl.sink_completed(serial);
                            }
                        ));
                    }
                }
            );
        }
    };
}

#endif //REPLAY_HPP
