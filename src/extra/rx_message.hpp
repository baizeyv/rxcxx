//
// Created by baizeyv on 6/27/2025.
//

#ifndef RX_MESSAGE_HPP
#define RX_MESSAGE_HPP
#include "unit.h"
#include "../subjects/subject.hpp"

namespace rxcxx::extra {
    class rx_message {
    private:
        subjects::subject<unit> subject;
    public:
        rx_message() = default;

        void operator()() const {
            subject.as_subscriber().on_next({});
        }

        template<typename Operator>
        auto operator >>(Operator &&op) const {
            return subject.as_observable() >> std::forward<Operator>(op);
        }

        template<typename Operator>
        auto operator |(Operator &&op) const {
            return subject.as_observable() >> std::forward<Operator>(op);
        }

        void dispose() const {
            subject.as_subscriber().on_completed();
        }
    };
}

#endif //RX_MESSAGE_HPP
