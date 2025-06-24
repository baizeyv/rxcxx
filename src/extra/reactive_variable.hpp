//
// Created by baizeyv on 6/23/2025.
//

#ifndef REACTIVE_VARIABLE_H
#define REACTIVE_VARIABLE_H

#include "../rx-includes.hpp"

namespace rxcxx {
    template<class ValueType>
    class reactive_variable {
    public:
        using this_type = reactive_variable<ValueType>;
        using value_type = ValueType;

    private:
        subjects::behavior<value_type> subject;

    public:
        /**
         * * 左值引用构造函数
         * @param p_value
         */
        explicit reactive_variable(const value_type &p_value) : subject(p_value) {
        }

        /**
         * * 右值引用构造函数
         * @param p_value
         */
        explicit reactive_variable(const value_type &&p_value) : subject(std::move(p_value)) {
        }

        value_type value() const {
            return subject.get_value();
        }

        /**
         * * 完美转发重载=
         * @tparam U
         * @param other
         * @return
         */
        template<typename U>
        reactive_variable &operator=(U &&other) {
            if (value() == other)
                return *this;

            subject.get_subscriber().on_next(std::forward<U>(other));

            return *this;
        }

        bool operator==(const this_type& other) const {
            return value() == other.value();
        }

        bool operator!=(const this_type& other) const {
            return !(this->value() == other.value());
        }

        bool operator<(const this_type& other) const {
            return value() < other.value();
        }

        bool operator<=(const this_type& other) const {
            return this->value() < other.value() || this->value() == other.value();
        }

        bool operator>(const this_type& other) const {
            return value() > other.value();
        }

        bool operator>=(const this_type& other) const {
            return value() >= other.value();
        }

        /**
         * * subscribe without init value
         * @tparam Operator
         * @param op
         * @return
         */
        template<typename Operator>
        auto operator >>(Operator &&op) const {
            return subject.get_observable() >> skip(1) >> std::forward<Operator>(op);
        }

        /**
         * * subscribe with init value
         * @tparam Operator
         * @param op
         * @return
         */
        template<typename Operator>
        auto operator >>=(Operator &&op) const {
            return subject.get_observable() >> std::forward<Operator>(op);
        }

        template<typename Operator>
        auto operator |(Operator &&op) const {
            return subject.get_observable() >> skip(1) >> std::forward<Operator>(op);
        }

        template<typename Operator>
        auto operator |=(Operator &&op) const {
            return subject.get_observable() >> std::forward<Operator>(op);
        }
    };
};

#endif //REACTIVE_VARIABLE_H
