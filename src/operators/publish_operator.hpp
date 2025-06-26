//
// Created by baizeyv on 6/26/2025.
//

#ifndef PUBLISH_OPERATOR_HPP
#define PUBLISH_OPERATOR_HPP
#include "../observables/connectable_observable.hpp"

namespace rxcxx::operators {
    inline auto publish() noexcept {
        return [](auto source) {
            using Source = decltype(source);
            using Out = typename Source::value_type;
            return observables::connectable<>::make_connectable<Out>(source);
        };
    }
}

#endif //PUBLISH_OPERATOR_HPP
