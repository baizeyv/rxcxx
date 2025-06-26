//
// Created by baizeyv on 6/25/2025.
//

#ifndef ERROR_OBSERVABLE_HPP
#define ERROR_OBSERVABLE_HPP
#include "../observable.hpp"

namespace rxcxx::observables {
    template<typename T>
    inline observable<T> error(std::exception_ptr err) noexcept {
        return observable<>::make_observable<T>([err](observer<T> s) {
            s.on_error(err);
        });
    }

    template<typename T, typename ERR>
    inline observable<T> error(const ERR & err) {
        return error<T>(std::make_exception_ptr(err));
    }
}

#endif //ERROR_OBSERVABLE_HPP
