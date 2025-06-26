//
// Created by baizeyv on 6/25/2025.
//

#ifndef OBSERVABLE_HPP
#define OBSERVABLE_HPP
#include <algorithm>

#include "observer.hpp"
#include "subscription.hpp"

namespace rxcxx {
    template<class T = void>
    class observable;

    // * ╭──────────────────────────────────────╮
    // * │             编译时检测               │
    template<typename T>
    struct is_observable : std::false_type {
    };

    template<typename T>
    struct is_observable<observable<T> > : std::true_type {
    };

    // * ╰──────────────────────────────────────╯

    template<>
    class observable<void> {
    public:
        template<typename T>
        static auto make_observable(const typename observable<T>::source_type &f) noexcept {
            return observable<T>(std::make_shared<typename observable<T>::source_type>(f));
        }

        template<typename T>
        static auto make_observable(typename observable<T>::source_type &&f) noexcept {
            return observable<T>(
                std::make_shared<typename observable<T>::source_type>(std::move(f))
            );
        }

        // TODO: INC
    };

    template<class T>
    class observable {
    public:
        using value_type = T;
        using observer_type = observer<value_type>;
        using subscriber_type = observer<value_type>;
        using source_type = std::function<void(subscriber_type)>;
        using source_ptr = std::shared_ptr<source_type>;
        using next_type = typename observer_type::next_type;
        using error_type = typename observer_type::error_type;
        using completed_type = typename observer_type::completed_type;

    private:
        source_ptr source;

    protected:
        observable() = default;

    public:
        explicit observable(source_ptr source) : source(source) {
        }

        virtual subscription subscribe(observer_type ob) const noexcept {
            (*source)(ob);
            return subscription([ob]() {
                                    ob.dispose();
                                }, [ob]() {
                                    return ob.is_no_disposed();
                                });
        }

        subscription subscribe(const next_type &n = {}, const error_type &e = {},
                               const completed_type &c = {}) const noexcept {
            return subscribe(observer<value_type>(n, e, c));
        }

        template<typename N, typename E, typename C>
        subscription subscribe(N &&n, E &&e, C &&c) const noexcept {
            return subscribe(observer<value_type>(std::forward<N>(n), std::forward<E>(e), std::forward<C>(c)));
        }

        template<typename Operator>
        auto operator |(Operator &&f) const noexcept {
            return f(*this);
        }

        template<typename Operator>
        auto operator >>(Operator &&f) const noexcept {
            return f(*this);
        }

        auto as_dynamic() const noexcept -> observable<value_type> {
            return *this;
        }

        // TODO: operators INC

    };


    // TODO: INC
}

#endif //OBSERVABLE_HPP
