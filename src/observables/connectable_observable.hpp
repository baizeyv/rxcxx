//
// Created by baizeyv on 6/26/2025.
//

#ifndef CONNECTABLE_OBSERVABLE_HPP
#define CONNECTABLE_OBSERVABLE_HPP
#include "../observable.hpp"

namespace rxcxx::observables {
    template<typename T = void>
    class connectable;

    template<>
    class connectable<void> {
    public:
        template<typename T>
        static auto make_connectable(observable<T> src) noexcept {
            return connectable<T>(src);
        }
    };

    template<class T>
    class connectable : public observable<T> {
        friend class connectable<>;

    public:
        using value_type = typename observable<T>::value_type;
        using source_observable = observable<value_type>;
        using observer_type = typename observable<T>::observer_type;
        using serial_type = int32_t;
        using observer_map = std::map<serial_type, observer_type>; // # 为了使执行顺序和subscribe的顺序相同,使用map,否则可以用unordered_map

    private:
        struct member {
            source_observable source;
            observer_map observers;
            std::recursive_mutex mtx;
            serial_type serial;
            subscription subscription;

            explicit member(source_observable &&source) : source(std::move(source)), serial(0) {
            }

            explicit member(const source_observable &source) : source(source), serial(0) {
            }
        };

        std::shared_ptr<member> mem;

    public:
        connectable() = default;

        explicit connectable(const source_observable &source) noexcept : mem(std::make_shared<member>(source)) {
        }

        connectable(source_observable &&source) noexcept : mem(std::make_shared<member>(std::move(source))) {
        }

        virtual subscription subscribe(observer_type ob) const noexcept override {
            auto m = mem;

            const auto serial = [&] {
                std::lock_guard<std::recursive_mutex> lock(m->mtx);
                return m->serial++;
            }();

            m->observers.insert({serial, ob});

            return subscription([m, serial] {
                std::lock_guard<std::recursive_mutex> lock(m->mtx);
                auto it = m->observers.find(serial);
                if (it != m->observers.end()) {
                    m->observers.erase(it);
                    // enable below if not forever
                    // if(m->observers.size() == 0){
                    //   m->subscription.dispose();
                    // }
                }
            }, [m, serial] {
                std::lock_guard<std::recursive_mutex> lock(m->mtx);
                auto it = m->observers.find(serial);
                if (it != m->observers.end()) {
                    return it->second.is_no_disposed();
                } else {
                    return false;
                }
            });
        }

        subscription connect() const noexcept {
            auto m = mem;

            auto collect = [m](const bool clear) {
                std::lock_guard<std::recursive_mutex> lock(m->mtx);
                std::vector<observer_type> ret;
                std::for_each(m->observers.begin(), m->observers.end(), [&ret](auto &it) {
                    ret.push_back(it.second);
                });
                if (clear)
                    m->observers.clear();
                return ret;
            };

            auto sbsc = m->source.subscribe(
                [collect](const value_type &x) {
                    auto obs = collect(false);
                    std::for_each(obs.begin(), obs.end(), [&x](auto &ob) {
                        ob.on_next(x);
                    });
                },
                [collect](std::exception_ptr err) {
                    auto obs = collect(true);
                    std::for_each(obs.begin(), obs.end(), [&err](auto &ob) {
                        ob.on_error(err);
                    });
                },
                [collect]() {
                    auto obs = collect(true);
                    std::for_each(obs.begin(), obs.end(), [](auto &ob) {
                        ob.on_completed();
                    });
                }
            );

            std::lock_guard<std::recursive_mutex> lock(m->mtx);
            auto sbsc2 = subscription(
                [sbsc, m] {
                    sbsc.dispose();
                    std::lock_guard<std::recursive_mutex> lock(m->mtx);
                    m->observers.clear();
                    m->subscription = subscription();
                },
                [sbsc] {
                    return sbsc.is_no_disposed();
                }
            );

            m->subscription = sbsc2;
            return sbsc2;
        }
    };
}

#endif //CONNECTABLE_OBSERVABLE_HPP
