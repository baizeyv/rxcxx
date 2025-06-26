//
// Created by baizeyv on 6/25/2025.
//

#ifndef SUBSCRIPTION_HPP
#define SUBSCRIPTION_HPP
#include <functional>
#include <memory>

namespace rxcxx {
    class subscription {
    private:
        struct member {
            const std::function<void()> dispose;
            const std::function<bool()> is_no_disposed;

            template<typename Dis, typename IsND>
            member(Dis &&dis, IsND &&is_no_disposed) noexcept : dispose(std::forward<Dis>(dis)),
                                                                is_no_disposed(std::forward<IsND>(is_no_disposed)) {
            }
        };

        std::shared_ptr<member> mem;

    public:
        subscription() noexcept = default;

        template<typename Dis, typename IsND>
        subscription(Dis &&dis, IsND &&is_no_disposed) noexcept : mem(
            std::make_shared<member>(std::forward<Dis>(dis), std::forward<IsND>(is_no_disposed))) {
        }

        subscription(const subscription& src) noexcept : mem(src.mem) {}

        const subscription& operator = (const subscription& src) {
            mem = src.mem;
            return *this;
        }

        void dispose() const noexcept {
            if (const auto m = mem; m && m->dispose) {
                m->dispose();
            }
        }

        bool is_no_disposed() const noexcept {
            const auto m = mem;
            return m && m->is_no_disposed ? m->is_no_disposed() : false;
        }
    };
}

#endif //SUBSCRIPTION_HPP
