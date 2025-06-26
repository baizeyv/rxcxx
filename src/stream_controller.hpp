//
// Created by baizeyv on 6/25/2025.
//

#ifndef STREAM_CONTROLLER_HPP
#define STREAM_CONTROLLER_HPP
#include <map>


namespace rxcxx {

    template<class T = void>
    struct observer;

    /**
     * * 操作流控制器
     * @tparam T
     */
    template<class T>
    class stream_controller {
    public:
        using value_type = T;
        using subscriber_type = observer<value_type>;
        using serial_type = int32_t;
        using disposable_map = std::map<serial_type, std::function<void()> >;
        using on_finalize_t = std::function<void()>;
        using on_finalize_ptr = std::shared_ptr<on_finalize_t>;

    private:
        struct inner {
            subscriber_type subscriber;
            std::recursive_mutex mtx;
            serial_type serial;
            disposable_map disposables;
            on_finalize_ptr on_finalize;

            explicit inner(subscriber_type s) noexcept : subscriber(s), serial(0) {
            }
        };

        mutable std::shared_ptr<inner> mem;

        explicit stream_controller(std::shared_ptr<inner> mem) noexcept : mem(mem) {
        }

        stream_controller() = delete;

    public:
        explicit stream_controller(subscriber_type subscriber) noexcept {
            mem = std::make_shared<inner>(subscriber);
            subscriber.set_on_dispose([inner = mem] {
                stream_controller(inner).finalize();
            });
        }

        template<typename F>
        void set_on_finalize(F &&f) const noexcept {
            std::lock_guard<std::recursive_mutex> lock(mem->mtx);
            mem->on_finalize = std::make_shared<on_finalize_t>(std::forward<F>(f));
        }

        template<typename In>
        observer<In> make_observer(std::function<void(serial_type, const In &)> n,
                                   std::function<void(serial_type, std::exception_ptr)> e,
                                   std::function<void(serial_type)> c) const noexcept {
            const auto serial = [&] {
                std::lock_guard<std::recursive_mutex> lock(mem->mtx);
                return mem->serial++;
            }();
            auto ob = observer<In>([n, serial](const In &value) {
                                       n(serial, value);
                                   }, [e, serial](std::exception_ptr err) {
                                       e(serial, err);
                                   }, [c, serial]() {
                                       c(serial);
                                   });
            {
                std::lock_guard<std::recursive_mutex> lock(mem->mtx);
                mem->disposables.insert({serial, [ob] { ob.dispose(); }});
            }
            return ob;
        }

        void sink_next(const value_type& value) const noexcept {
            if (mem->subscriber.is_no_disposed()) {
                mem->subscriber.on_next(value);
            } else {
                finalize();
            }
        }

        void sink_next(value_type&& value) const noexcept {
            if (mem->subscriber.is_no_disposed()) {
                mem->subscriber.on_next(std::move(value));
            } else {
                finalize();
            }
        }

        void sink_error(std::exception_ptr err) const noexcept {
            if (mem->subscriber.is_no_disposed()) {
                mem->subscriber.on_error(err);
                finalize();
            } else {
                finalize();
            }
        }

        void sink_completed(serial_type serial) const noexcept {
            if (mem->subscriber.is_no_disposed()) {
                const auto done_all = [&] {
                    std::lock_guard<std::recursive_mutex> lock(mem->mtx);
                    mem->disposables.erase(serial);
                    return mem->disposables.size() == 0;
                }();
                if (done_all) {
                    mem->subscriber.on_completed();
                    finalize();
                }
            } else {
                finalize();
            }
        }

        void sink_completed_force() const noexcept {
            if (mem->subscriber.is_no_disposed()) {
                mem->subscriber.on_completed();
            }
            finalize();
        }

        void upstream_abort_observe(serial_type serial) const noexcept {
            const auto f = [&] {
                std::lock_guard<std::recursive_mutex> lock(mem->mtx);
                auto it = mem->disposables.find(serial);
                if (it == mem->disposables.end()) {
                    return on_finalize_t();
                }
                auto fn = it->second;
                mem->disposables.erase(it);
                return fn;
            }();
            if (f) {
                f();
            }
        }

        void finalize() const noexcept {
            std::lock_guard<std::recursive_mutex> lock(mem->mtx);
            for (auto it = mem->disposables.begin(); it != mem->disposables.end(); ++it) {
                it->second();
            }
            mem->disposables.clear();
            if (mem->subscriber.is_no_disposed()) {
                mem->subscriber.dispose();
            }
            if (mem->on_finalize && *mem->on_finalize) {
                (*mem->on_finalize)();
            }
            mem->on_finalize.reset();
        }

        bool is_no_disposed() const noexcept {
            return mem->subscriber.is_no_disposed();
        }
    };
}

#endif //STREAM_CONTROLLER_HPP
