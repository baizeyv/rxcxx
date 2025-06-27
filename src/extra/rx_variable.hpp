//
// Created by baizeyv on 6/27/2025.
//

#ifndef RX_VARIABLE_HPP
#define RX_VARIABLE_HPP
#include "../operators/distinct_until_changed_operator.hpp"
#include "../operators/skip_operator.hpp"
#include "../subjects/behavior.hpp"

namespace rxcxx::extra {
    template<class T>
    class rx_variable {
    public:
        using value_type = T;
        using this_type = rx_variable<T>;

    private:
        subjects::behavior<value_type> subject;

    public:
        /**
         * * 左值引用构造函数
         * @param p_value
         */
        explicit rx_variable(const value_type &p_value) noexcept : subject(p_value) {
        }

        /**
         * * 右值引用构造函数
         * @param p_value
         */
        rx_variable(rx_variable &&p_value) noexcept : subject(std::move(p_value)) {
        }

        value_type value() const {
            return subject.value();
        }

        /**
         * * 完美转发重载=
         * @tparam U
         * @param other
         * @return
         */
        template<typename U>
        rx_variable &operator =(U &&other) {
            subject.as_subscriber().on_next(std::forward<U>(other));
            return *this;
        }

        template<typename U>
        const rx_variable &operator =(U &&other) const {
            subject.as_subscriber().on_next(std::forward<U>(other));
            return *this;
        }

        bool operator !=(const this_type &other) const {
            return value() != other.value();
        }

        bool operator ==(const this_type &other) const {
            return !(*this != other);
        }

        bool operator <(const this_type &other) const {
            return value() < other.value();
        }

        bool operator <=(const this_type &other) const {
            return *this < other || *this == other;
        }

        bool operator >(const this_type &other) const {
            return value() > other.value();
        }

        bool operator >=(const this_type &other) const {
            return *this > other || *this == other;
        }

        template<typename Operator>
        auto operator >>(Operator &&op) const {
            return subject.as_observable() >> operators::skip(1) >> operators::distinct_until_changed() >> std::forward<
                       Operator>(op);
        }

        template<typename Operator>
        auto operator |(Operator &&op) const {
            return subject.as_observable() >> operators::skip(1) >> operators::distinct_until_changed() >> std::forward<
                       Operator>(op);
        }

        template<typename Operator>
        auto operator >>=(Operator &&op) const {
            return subject.as_observable() >> operators::distinct_until_changed() >> std::forward<
                       Operator>(op);
        }

        template<typename Operator>
        auto operator |=(Operator &&op) const {
            return subject.as_observable() >> operators::distinct_until_changed() >> std::forward<
                       Operator>(op);
        }

        template<typename U>
        rx_variable<U> operator+(const U &&i) = delete;

        template<typename U>
        rx_variable<U> operator-(const U &&i) = delete;

        template<typename U>
        rx_variable<U> operator*(const U &&i) = delete;

        template<typename U>
        rx_variable<U> operator/(const U &&i) = delete;

        void dispose() const {
            subject.as_subscriber().on_completed();
        }

    };
}

#endif //RX_VARIABLE_HPP
