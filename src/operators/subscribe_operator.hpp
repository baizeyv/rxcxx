//
// Created by baizeyv on 6/25/2025.
//

#ifndef SUBSCRIBE_OPERATOR_HPP
#define SUBSCRIBE_OPERATOR_HPP
#include "../observer.hpp"

namespace rxcxx::operators {
    template<typename T>
    auto subscribe(const observer<T> &ob) {
        return [ob](auto &&src) mutable {
            return src.subscribe(std::move(ob));
        };
    }

    template<typename T>
    auto subscribe(const observer<T> &&ob) {
        return [ob = std::move(ob)](auto &&src) mutable {
            // # src 是 observable
            return src.subscribe(std::move(ob));
        };
    }

    template<typename ON_NEXT>
    auto subscribe(ON_NEXT &&next, const observer<>::error_type &error,
                   const observer<>::completed_type &completed) noexcept {
        return [next = std::forward<ON_NEXT>(next), error, completed](auto &&src) mutable {
            return src.subscribe({std::move(next), std::move(error), std::move(completed)});
        };
    }

    template<typename ON_NEXT>
    auto subscribe(ON_NEXT&& next) noexcept {
        return subscribe(std::forward<ON_NEXT>(next), {}, {});
    }
}

#endif //SUBSCRIBE_OPERATOR_HPP
