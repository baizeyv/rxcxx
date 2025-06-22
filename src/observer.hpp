//
// Created by baizeyv on 6/21/2025.
//

#ifndef OBSERVER_H
#define OBSERVER_H
#include "predef.hpp"
#include "utils.h"
#include "../rxcpp/operators/rx-buffer_time_count.hpp"

namespace rxcxx
{
    template <class T>
    struct observer_base
    {
        using value_type = T;
        using observer_tag = tag_observer;
    };

    namespace detail
    {
        // * ╭───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╮

        /**
         * ! ╭────────────╮
         * ! │ 调用转发类 │
         * ! ╰────────────╯
         */

        /**
         * * 调用on_next无事发生
         * @tparam T 
         */
        template <class T>
        struct on_next_empty
        {
            void operator()(const T&) const
            {
            }
        };

        struct on_error_empty
        {
            void operator()(util::error_ptr) const
            {
                // ! 直接终止程序
                // error implicitly ignored, abort
                std::terminate();
            }
        };

        /**
         * * on_error时忽略异常
         */
        struct on_error_ignore
        {
            void operator()(util::error_ptr) const
            {
            }
        };

        struct on_completed_empty
        {
            void operator()() const
            {
            }
        };

        /**
         * * 向下传递on_next
         * @tparam T 
         * @tparam State 
         * @tparam OnNext 
         */
        template <class T, class State, class OnNext>
        struct on_next_forward
        {
            using state_t = util::decay_t<State>;
            using on_next_t = util::decay_t<OnNext>;
            on_next_t on_next;

            on_next_forward() : on_next()
            {
            }

            explicit on_next_forward(on_next_t next) : on_next(std::move(next))
            {
            }

            /**
             * * 重载函数调用 (左值引用)
             * @param s 
             * @param t 
             */
            void operator()(state_t& s, const T& t) const
            {
                on_next(s, t);
            }

            /**
             * * 重载函数调用 (右值引用)
             * @param s 
             * @param t 
             */
            void operator()(state_t& s, T&& t) const
            {
                on_next(s, std::move(t));
            }
        };

        /**
         * * 向下传递on_next (模板特化)
         * @tparam T 
         * @tparam State 
         */
        template <class T, class State>
        struct on_next_forward<T, State, void>
        {
            using state_t = util::decay_t<State>;

            explicit on_next_forward()
            {
            }

            /**
             * * 重载函数调用 (左值引用)
             * @param s 
             * @param t 
             */
            void operator()(state_t& s, const T& t) const
            {
                s.on_next(t);
            }

            /**
             * * 重载函数调用 (右值引用)
             * @param s 
             * @param t 
             */
            void operator()(state_t& s, T&& t) const
            {
                s.on_next(std::move(t));
            }
        };

        template <class State, class OnError>
        struct on_error_forward
        {
            using state_t = util::decay_t<State>;
            using on_error_t = util::decay_t<OnError>;
            on_error_t on_error;

            on_error_forward() : on_error()
            {
            }

            explicit on_error_forward(on_error_t error) : on_error(std::move(error))
            {
            }

            void operator(state_t& s, util::error_ptr ep) const
            {
                on_error(s, ep);
            }
        };

        template <class State>
        struct on_error_forward<State, void>
        {
            using state_t = util::decay_t<State>;

            explicit on_error_forward()
            {
            }

            void operator(state_t& s, util::error_ptr ep) const
            {
                s.on_error(ep);
            }
        };

        template <class State, class OnCompleted>
        struct on_completed_forward
        {
            using state_t = util::decay_t<State>;
            using on_completed_t = util::decay_t<OnCompleted>;
            on_completed_t on_completed;

            on_completed_forward() : on_completed()
            {
            }

            explicit on_completed_forward(on_completed_t on_completed) : on_completed(std::move(on_completed))
            {
            }

            void operator()(state_t& s) const
            {
                on_completed(s);
            }
        };

        template <class State>
        struct on_completed_forward<State, void>
        {
            on_completed_forward()
            {
            }

            void operator()(state_t& s) const
            {
                s.on_completed();
            }
        };

        // * ╰───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╯

        // * ╭───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╮

        /**
         * ! ╭──────────────╮
         * ! │ 编译时检测类 │
         * ! ╰──────────────╯
         */

        /**
         * * 编译时检测是否是对应的on_next
         * @tparam T 
         * @tparam F 
         */
        template <class T, class F>
        struct is_on_next_of
        {
            struct not_void
            {
            };

            // ! 判断某个函数类型 CF 是否可以用参数类型 CT 调用，并推导出返回值类型。
            template <class CT, class CF>
            static auto check(int) -> decltype(std::declval<CF>()(std::declval<CT>()));
            template <class CT, class CF>
            static not_void check(...);

            using detail_result = decltype(check<T, util::decay_t<F>>(0));
            static const bool value = std::is_same_v<detail_result, void>;
        };

        template <class F>
        struct is_on_error
        {
            struct not_void
            {
            };

            template <class CF>
            static auto check(int) -> decltype(std::declval<CF>()(std::declval<util::error_ptr>()));
            template <class CF>
            static not_void check(...);

            static const bool value = std::is_same_v<decltype(check<util::decay_t<F>>(0)), void>;
        };

        template <class State, class F>
        struct is_on_error_for
        {
            struct not_void
            {
            };

            template <class CF>
            static auto check(int) -> decltype(std::declval<CF>()(std::declval<State>(), std::declval<util::error_ptr>()));
            template <class CF>
            static not_void check(...);

            static const bool value = std::is_same_v<decltype(check<util::decay_t<F>>(0)), void>;
        };

        template <class F>
        struct is_on_completed
        {
            struct not_void
            {
            };

            template <class CF>
            static auto check(int) -> decltype(std::declval<CF>());
            template <class CF>
            static not_void check(...);

            static const bool value = std::is_same_v<decltype(check<util::decay_t<F>>(0)), void>;
        };

        // * ╰───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╯
    }

    template <class T, class State, class OnNext, class OnError, class OnCompleted>
    class observer : public observer_base<T>
    {
    public:
        using this_type = observer<T, State, OnNext, OnError, OnCompleted>;
        using state_t = util::decay_t<State>;
        using on_next_t = typename std::conditional_t<!std::is_same_v<void, OnNext>, util::decay_t<OnNext>, detail::on_next_forward<T, State, OnNext>>
        ;
        using on_error_t = typename std::conditional_t<!std::is_same_v<void, OnError>, util::decay_t<OnError>, detail::on_error_forward<T, OnError>>;
        using on_completed_t = typename std::conditional_t<!std::is_same_v<void, OnCompleted>, util::decay_t<OnCompleted>,
                                                           detail::on_completed_forward<T, OnCompleted>>;

    private:
        // ! mutable -> 允许在const成员函数中修改
        mutable state_t state;
        on_next_t on_next_v;
        on_error_t on_error_v;
        on_completed_t on_completed_v;

    public:
        explicit observer(state_t s, on_next_t n = on_next_t(), on_error_t e = on_error_t(), on_completed_t c = on_completed_t()) :
            state(std::move(s)), on_next_v(std::move(n)), on_error_v(std::move(e)), on_completed_v(std::move(c))
        {
        }

        explicit observer(state_t s, on_next_t n, on_completed_t c) : state(std::move(s)), on_next_v(std::move(n)), on_error_v(on_error_t()),
                                                                      on_completed_v(std::move(c))
        {
        }

        /**
         * * 拷贝构造
         * @param o 
         */
        observer(const this_type& o) : state(o.state), on_next_v(o.on_next_v), on_error_v(o.on_error_v), on_completed_v(o.on_completed_v)
        {
        }


        /**
         * * 移动构造
         * @param o 
         */
        observer(this_type&& o) : state(std::move(o.state)), on_next_v(std::move(o.on_next_v)), on_error_v(std::move(o.on_error_v)),
                                  on_completed_v(std::move(o.on_completed_v))
        {
        }

        this_type& operator=(this_type o)
        {
            state = std::move(o.state);
            on_next_v = std::move(o.on_next_v);
            on_error_v = std::move(o.on_error_v);
            on_completed_v = std::move(o.on_completed_v);
            return *this;
        }

        /**
         * * 左值引用版on_next
         * @param t 
         */
        void on_next(const T& t) const
        {
            on_next_v(state, t);
        }

        /**
         * * 右值引用版on_next
         * @param t 
         */
        void on_next(T&& t) const
        {
            on_next_v(state, std::move(t));
        }

        void on_error(util::error_ptr e) const
        {
            on_error_v(state, e);
        }

        void on_completed() const
        {
            on_completed_v(state);
        }

        observer<T> as_dynamic() const 
        {
            return observer<T>(*this);
        }
    };

    template <class T, class OnNext, class OnError, class OnCompleted>
    class observer<T, stateless_observer_tag, OnNext, OnError, OnCompleted> : public observer_base<T>
    {
    public:
        using this_type = observer<T, stateless_observer_tag, OnNext, OnError, OnCompleted>;
        using on_next_t = typename std::conditional_t<!std::is_same_v<void, OnNext>, util::decay_t<OnNext>, detail::on_next_forward<T, State, OnNext>>
        ;
        using on_error_t = typename std::conditional_t<!std::is_same_v<void, OnError>, util::decay_t<OnError>, detail::on_error_forward<T, OnError>>;
        using on_completed_t = typename std::conditional_t<!std::is_same_v<void, OnCompleted>, util::decay_t<OnCompleted>,
                                                           detail::on_completed_forward<T, OnCompleted>>;

    private:
        on_next_t on_next_v;
        on_error_t on_error_v;
        on_completed_t on_completed_v;

    public:
        static_assert(detail::is_on_next_of<T, on_next_t>::value, "Function supplied for on_next must be a function with the signature void(T);");
        static_assert(detail::is_on_error<on_error_t>::value,
                      "Function supplied for on_error must be a function with the signature void(rxu::error_ptr);");
        static_assert(detail::is_on_completed<on_completed_t>::value,
                      "Function supplied for on_completed must be a function with the signature void();");

        observer() : on_next_v(on_next_t()), on_error_v(on_error_t()), on_completed_v(on_completed_t())
        {
        }

        explicit observer(on_next_t n, on_error_t e = on_error_t(), on_completed_t c = on_completed_t()) : on_next_v(std::move(n)),
            on_error_v(std::move(e)),
            on_completed_v(std::move(c))
        {
        }

        /**
         * * 拷贝构造
         * @param o 
         */
        observer(const this_type& o) : on_next_v(o.on_next_v), on_error_v(o.on_error_v), on_completed_v(o.on_completed_v)
        {
        }

        /**
         * * 移动构造
         * @param o 
         */
        observer(this_type&& o) : on_next_v(std::move(o.on_next_v)), on_error_v(std::move(o.on_error_v)), on_completed_v(std::move(o.on_completed_v))
        {
        }

        this_type& operator=(this_type o)
        {
            on_next_v = std::move(o.on_next_v);
            on_error_v = std::move(o.on_error_v);
            on_completed_v = std::move(o.on_completed_v);
            return *this;
        }

        void on_next(const T& t) const
        {
            on_next_v(t);
        }

        void on_next(T&& t) const
        {
            on_next_v(std::move(t));
        }

        void on_error(util::error_ptr e) const
        {
            on_error_v(e);
        }

        void on_completed() const
        {
            on_completed_v();
        }
        
        observer<T> as_dynamic() const 
        {
            return observer<T>(*this);
        }
    };

    namespace detail
    {
        /**
         * * 真正的观察者基类
         * @tparam T 
         */
        template<class T>
        struct virtual_observer : public std::enable_shared_from_this<virtual_observer<T>>
        {
            virtual ~virtual_observer() {}
            virtual void on_next(const T&) const {};
            virtual void on_next(T&&) const {};
            virtual void on_error(util::error_ptr) const {};
            virtual void on_completed() const {};
        };

        template<class T, class Observer>
        struct specific_observer : public virtual_observer<T>
        {
            Observer destination;

            explicit specific_observer(Observer o) : destination(std::move(o)){}

            void on_next(const T& t) const override
            {
                destination.on_next(t);
            }
            void on_next(T&& t) const override{
                destination.on_next(std::move(t));
            }
            void on_error(util::error_ptr e) const override{
                destination.on_error(e);
            }
            void on_completed() const override {
                destination.on_completed();
            }
        };
    }

    template<class T>
    class observer<T, void, void, void, void> : public observer_base<T>
    {
    public:
        using dynamic_observer_tag = tag_dynamic_observer;
    private:
        using this_type = observer<T, void, void, void, void>;
        using base_type = observer_base<T>;
        using virtual_observer = detail::virtual_observer<T>;

        std::shared_ptr<virtual_observer> destination;

        template<class Observer>
        static std::shared_ptr<virtual_observer> make_destination(Observer o)
        {
            return std::make_shared<detail::specific_observer<T, Observer>>(std::move(o));
        }
    public:
        observer(){}
        /**
         * * 拷贝构造
         * @param o 
         */
        observer(const this_type& o) : destination(o.destination){}
        /**
         * * 移动构造
         * @param o 
         */
        observer(this_type&& o) : destination(std::move(o.destination)){}

        template<class Observer>
        explicit observer(Observer o) : destination(make_destination(std::move(o))){}

        this_type& operator=(this_type o)
        {
            destination = std::move(o.destination);
            return *this;
        }

        /**
         * * 完美转发on_next
         * @tparam V 
         * @param v 
         */
        template<class V>
        void on_next(V&& v) const
        {
            if (destination)
                destination->on_next(std::forward<V>(v));
        }

        void on_error(util::error_ptr e) const
        {
            if (destination)
                destination->on_error(e);
        }

        void on_completed() const
        {
            if (destination)
                destination->on_completed();
        }
        
        observer<T> as_dynamic() const {
            return *this;
        }
    };

    template<class T, class DefaultOnError = detail::on_error_empty>
    auto make_observer() -> observer<T, stateless_observer_tag, detail::on_next_empty<T>, DefaultOnError>
    {
        return observer<T, stateless_observer_tag, detail::on_next_empty<T>, DefaultOnError>();
    }
}

#endif //OBSERVER_H
