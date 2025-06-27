//
// Created by baizeyv on 6/26/2025.
//

#ifndef BLOCKING_OBSERVABLES_HPP
#define BLOCKING_OBSERVABLES_HPP
#include <condition_variable>
#include <deque>
#include <numeric>
#include <stdexcept>
#include <type_traits>

#include "../observable.hpp"

namespace rxcxx::observables {
    template<typename T = void>
    class blocking;
    template<typename T>
    class calculable_blocking;

    template<>
    class blocking<void> {
    public:
        template<
            typename T,
            std::enable_if_t<!std::is_arithmetic<T>::value, bool>  = true
        >
        static auto make_blocking(observable<T> src) noexcept {
            return blocking<T>(src);
        }

        template<
            typename T,
            std::enable_if_t<std::is_arithmetic<T>::value, bool>  = true
        >
        static auto make_blocking(observable<T> src) noexcept {
            return calculable_blocking<T>(src);
        }
    };

    class empty_error : public std::runtime_error {
    public:
        empty_error(const std::string &msg) noexcept : runtime_error(msg) {
        }
    };

    template<typename T>
    class blocking : public observable<T> {
        friend class blocking<>;

    public:
        using value_type = typename observable<T>::value_type;
        using observer_type = typename observable<T>::observer_type;
        using this_type = blocking<value_type>;

    protected:
        mutable observable<value_type> src_;
        mutable std::deque<value_type> queue_;
        mutable std::exception_ptr error_;
        mutable bool subscribed_;
        mutable bool completed_;
        mutable bool first_;
        mutable std::mutex mtx;
        mutable std::condition_variable cv;

        blocking(observable<value_type> src) noexcept : src_(src), error_(nullptr), subscribed_(false), completed_(false), first_(false) {
        }

        void subscribe_all() const {
            if (subscribed_) return;
            subscribed_ = true;
            src_.subscribe(
                [&](const value_type &x) {
                    std::lock_guard<std::mutex> lock(mtx);
                    queue_.push_back(x);
                    first_ = true;
                    cv.notify_one();
                },
                [&](std::exception_ptr err) {
                    std::lock_guard<std::mutex> lock(mtx);
                    error_ = err;
                    cv.notify_one();
                },
                [&] {
                    std::lock_guard<std::mutex> lock(mtx);
                    completed_ = true;
                    cv.notify_one();
                }
            );
        }

    public:
        virtual subscription subscribe(observer_type ob) const noexcept override {
            std::unique_lock lock(mtx);
            subscribe_all();
            cv.wait(lock, [this] {
                return error_ || completed_;
            });
            std::for_each(std::begin(queue_), std::end(queue_), [ob](auto &&x) {
                ob.on_next(x);
            });
            if (error_) {
                ob.on_error(error_);
            } else {
                ob.on_completed();
            }
            return subscription(
                [] {
                },
                [] {
                    return false;
                }
            );
        }

        value_type first() const noexcept(false) {
            std::unique_lock lock(mtx);
            subscribe_all();
            cv.wait(lock, [this] {
                return first_;
            });
            if (queue_.empty()) {
                if (error_) {
                    std::rethrow_exception(error_);
                } else {
                    throw empty_error("empty");
                }
            } else {
                auto x = queue_.front();
                queue_.pop_front();
                return x;
            }
        }

        value_type last() const noexcept(false) {
            std::unique_lock lock(mtx);
            subscribe_all();
            cv.wait(lock, [this] {
                return error_ || completed_;
            });
            if (queue_.empty()) {
                if (error_) {
                    std::rethrow_exception(error_);
                } else {
                    throw empty_error("empty");
                }
            } else {
                auto x = queue_.back();
                queue_.pop_back();
                return x;
            }
        }

        std::size_t count() const noexcept(false) {
            std::unique_lock lock(mtx);
            subscribe_all();
            cv.wait(lock, [this] {
                return error_ || completed_;
            });
            return queue_.size();
        }
    };

    template<typename T>
    class calculable_blocking : public blocking<T> {
        friend class blocking<>;

    public:
        using value_type = typename blocking<T>::value_type;

    private:
        using base = blocking<T>;

        calculable_blocking(observable<T> src) noexcept : blocking<T>(src) {
        }

    public:
        value_type sum() const noexcept(false) {
            std::unique_lock lock(this->mtx);
            base::subscribe_all();
            this->cv.wait(lock, [this] {
                return this->error_ || this->completed_;
            });
            if (base::error_) std::rethrow_exception(base::error_);
            else if (base::queue_.empty()) throw empty_error("empty");
            return std::accumulate(std::cbegin(base::queue_), std::cend(base::queue_), 0);
        }

        double average() const noexcept(false) {
            std::unique_lock lock(this->mtx);
            base::subscribe_all();
            this->cv.wait(lock, [this] {
                return this->error_ || this->completed_;
            });
            if (base::error_) std::rethrow_exception(base::error_);
            else if (base::queue_.empty()) throw empty_error("empty");
            auto sum = std::accumulate(std::cbegin(base::queue_), std::cend(base::queue_), 0);
            return static_cast<double>(sum) / static_cast<double>(base::queue_.size());
        }

        value_type max() const noexcept(false) {
            std::unique_lock lock(this->mtx);
            base::subscribe_all();
            this->cv.wait(lock, [this] {
                return this->error_ || this->completed_;
            });
            if (base::error_) std::rethrow_exception(base::error_);
            else if (base::queue_.empty()) throw empty_error("empty");
            return *std::max_element(std::cbegin(base::queue_), std::cend(base::queue_));
        }

        value_type min() const noexcept(false) {
            std::unique_lock lock(this->mtx);
            base::subscribe_all();
            this->cv.wait(lock, [this] {
                return this->error_ || this->completed_;
            });
            if (base::error_) std::rethrow_exception(base::error_);
            else if (base::queue_.empty()) throw empty_error("empty");
            return *std::min_element(std::cbegin(base::queue_), std::cend(base::queue_));
        }
    };
}

#endif //BLOCKING_OBSERVABLES_HPP
