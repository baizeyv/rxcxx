//
// Created by baizeyv on 6/27/2025.
//

#ifndef RX_EVENT_HPP
#define RX_EVENT_HPP
#include "../subjects/subject.hpp"

namespace rxcxx::extra {
    template<class T>
    class rx_event {
    public:
        using event_type = T;

    private:
        subjects::subject<event_type> subject;

    public:
        rx_event() = default;

        template<typename U>
        rx_event &operator =(U &&other) {
            subject.as_subscriber().on_next(std::forward<U>(other));
            return *this;
        }

        template<typename U>
        const rx_event &operator =(U &&other) const {
            subject.as_subscriber().on_next(std::forward<U>(other));
            return *this;
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

#endif //RX_EVENT_HPP
