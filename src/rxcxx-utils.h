//
// Created by baizeyv on 6/25/2025.
//

#ifndef RXCXX_UTILS_H
#define RXCXX_UTILS_H
#include <type_traits>

namespace rxcxx {
    template<typename T>
    struct strip_const_ref {
        using type = typename std::remove_const_t<typename std::remove_reference_t<T> >;
    };

    template<typename F, typename... Args>
    struct lambda_return_type {
        using type = std::invoke_result_t<F, Args...>;
    };

    template<typename F, typename... Args>
    using lambda_return_type_t = typename lambda_return_type<F, Args...>::type;
}

#endif //RXCXX_UTILS_H
