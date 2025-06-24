//
// Created by baizeyv on 6/24/2025.
//

#ifndef SIMPLE_EVENT_H
#define SIMPLE_EVENT_H

#include "../rx-includes.hpp"

namespace rxcxx {
    template<class ValueType>
    class simple_event {
    public:
        using this_type = simple_event<ValueType>;
        using value_type = ValueType;
    private:
        rxcpp::subjects::subject<value_type> subject;

    public:
        simple_event() : subject() {}

        simple_event& operator=(simple_event& val) = delete;

        /**
         * * 完美转发触发事件
         * @tparam U
         * @param evt
         */
        template<class U>
        void fire(U&& evt) {
            subject.get_subscriber().on_next(std::forward<U>(evt));
        }

        void fire() {
            subject.get_subscriber().on_next(value_type());
        }

        /**
         * * subscribe
         * @tparam Operator
         * @param op
         * @return
         */
        template<typename Operator>
        auto operator >>(Operator &&op) const {
            return subject.get_observable() >> std::forward<Operator>(op);
        }

        template<typename Operator>
        auto operator |(Operator &&op) const {
            return subject.get_observable() >> std::forward<Operator>(op);
        }
    };
}

#endif //SIMPLE_EVENT_H
