//
// Created by baizeyv on 6/26/2025.
//

#ifndef BEHAVIOR_HPP
#define BEHAVIOR_HPP
#include "subject.hpp"

namespace rxcxx::subjects {
    template<class T>
    class behavior : public subject<T> {
    private:
        struct member {
            T last;
            std::mutex mtx;
            subscription subscription;

            explicit member(const T &last) : last(last) {
            }

            ~member() {
                subscription.dispose();
            }
        };

        std::shared_ptr<member> mem;

        void initialize() {
            auto m = mem;
            m->subscription = this->as_observable().subscribe(
                {
                    [m](const T &x) {
                        std::lock_guard<std::mutex> lock(m->mtx);
                        m->last = std::move(x);
                    },
                    [](std::exception_ptr _) {
                    },
                    []() {
                    }
                }
            );
        }

    public:
        explicit behavior(T &&initial_value) noexcept : mem(std::make_shared<member>(std::move(initial_value))) {
            initialize();
        }

        explicit behavior(const T &initial_value) noexcept : mem(std::make_shared<member>(initial_value)) {
            initialize();
        }

        virtual ~behavior() = default;

        T value() {
            return mem->last;
        }

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
                        sctl.sink_next([m]() {
                            std::lock_guard<std::mutex> lock(m->mtx);
                            return m->last;
                        }());
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

#endif //BEHAVIOR_HPP
