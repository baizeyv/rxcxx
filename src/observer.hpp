//
// Created by baizeyv on 6/25/2025.
//

#ifndef OBSERVER_HPP
#define OBSERVER_HPP
#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <bits/exception_ptr.h>

#include "stream_controller.hpp"

namespace rxcxx {

    template<class T>
    struct observer;

    template<>
    struct observer<void> {
        template<typename T>
        using next_type = std::function<void(const T &)>;

        using error_type = std::function<void(std::exception_ptr)>;
        using completed_type = std::function<void()>;
        using dispose_type = std::function<void()>;
    };

    template<class T>
    struct observer {
        // * ╭──────────╮
        // * │ 类型定义 │
        // * ╰──────────╯
        using value_type = T;
        using next_type = observer<>::next_type<value_type>;
        using error_type = observer<>::error_type;
        using completed_type = observer<>::completed_type;
        using dispose_type = observer<>::dispose_type;

        using next_ptr = std::shared_ptr<next_type>;
        using error_ptr = std::shared_ptr<error_type>;
        using completed_ptr = std::shared_ptr<completed_type>;
        using dispose_ptr = std::shared_ptr<dispose_type>;

    private:
        struct inner {
            std::recursive_mutex mtx;
            next_ptr next;
            error_ptr error;
            completed_ptr completed;
            dispose_ptr dispose;
            std::atomic_bool is_no_disposed;

            inner() : is_no_disposed(true) {}
        };

        mutable std::shared_ptr<inner> inner_ptr;

        auto fetch_and_reset_inner() const noexcept {
            std::lock_guard<std::recursive_mutex> lock(inner_ptr->mtx);
            auto e = inner_ptr->error;
            auto c = inner_ptr->completed;
            auto d = inner_ptr->dispose;
            inner_ptr->next.reset();
            inner_ptr->error.reset();
            inner_ptr->completed.reset();
            inner_ptr->dispose.reset();
            return std::make_tuple(e, c, d);
        }

        void set_disposed() const noexcept {
            inner_ptr->is_no_disposed = false;
        }

        template<typename Dis>
        void set_on_dispose(Dis &&foobar) const noexcept {
            inner_ptr->dispose = std::make_shared<dispose_type>(std::forward<Dis>(foobar));
        }

        friend class stream_controller<T>;

    public:
        template<typename N, typename E, typename C>
        observer(N &&n, E &&e, C &&c) noexcept {
            inner_ptr = std::make_shared<inner>();
            inner_ptr->next = std::make_shared<next_type>(std::forward<N>(n));
            inner_ptr->error = std::make_shared<error_type>(std::forward<E>(e));
            inner_ptr->completed = std::make_shared<completed_type>(std::forward<C>(c));
        }

        explicit observer(const next_type &n = {}, const error_type &e = {}, const completed_type &c = {}) noexcept {
            inner_ptr = std::make_shared<inner>();
            inner_ptr->next = std::make_shared<next_type>(n);
            inner_ptr->error = std::make_shared<error_type>(e);
            inner_ptr->completed = std::make_shared<completed_type>(c);
        }

        /**
         * * 右值引用on_next
         * @param value
         */
        void on_next(const value_type &value) const noexcept {
            auto nxt = inner_ptr->next;
            if (nxt && *nxt) {
                // # 调用这个function
                (*nxt)(value);
            }
        }

        /**
         * * 左值引用on_next
         * @param value
         */
        void on_next(const value_type &&value) const noexcept {
            auto nxt = inner_ptr->next;
            if (nxt && *nxt) {
                // # 调用这个function
                (*nxt)(std::move(value));
            }
        }

        void on_error(std::exception_ptr err) const noexcept {
            auto ecd = fetch_and_reset_inner();
            auto e = std::get<0>(ecd);
            auto d = std::get<2>(ecd);
            // # 调用error
            if (e && *e) {
                (*e)(err);
            }
            // # 调用dispose
            if (d && *d) {
                (*d)();
            }
            set_disposed();
        }

        void on_completed() const noexcept {
            auto ecd = fetch_and_reset_inner();
            auto c = std::get<1>(ecd);
            auto d = std::get<2>(ecd);
            // # 调用completed
            if (c && *c) {
                (*c)();
            }
            // # 调用dispose
            if (d && *d) {
                (*d)();
            }
            set_disposed();
        }

        void dispose() const noexcept {
            auto ecd = fetch_and_reset_inner();
            auto d = std::get<2>(ecd);
            if (d && *d) {
                (*d)();
            }
            set_disposed();
        }

        bool is_no_disposed() const noexcept {
            return inner_ptr->is_no_disposed;
        }
    };
}

#endif //OBSERVER_HPP
