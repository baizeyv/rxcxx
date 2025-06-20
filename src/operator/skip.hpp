//
// Created by baizeyv on 6/19/2025.
//

#ifndef SKIP_H
#define SKIP_H

// ! safe completed

template<class T>
class abs_observable;

template<class T>
class skip_observer final : public abs_observer<T> {
private:
    std::unique_ptr<abs_observer<T>> observer;
    int count;

public:
    skip_observer(std::unique_ptr<abs_observer<T>> observer, const int count) : observer(std::move(observer)), count(count) {
    }

protected:

    void on_complete_core(std::unique_ptr<result> rst) override {
        observer->on_complete(rst);
    }

    void on_next_core(T &p_value) override {
        if (count > 0) {
            --count;
        }
        else
            observer->on_next(p_value);
    }

    void on_error_core(std::runtime_error &error) override {
        observer->on_error(error);
    }
};


/**
 * * 跳过几次之后开始生效
 * @tparam T
 */
template <class T>
class skip final : public abs_observable<T> {
private:
    /**
     * * source 被观察者指针
     */
    std::unique_ptr<abs_observable<T>> source;

    /**
     * * 需要跳过的数量
     */
    int count;

public:
    skip(std::unique_ptr<abs_observable<T>> source, const int count) : source(std::move(source)), count(count) {

    }

protected:
    std::unique_ptr<disposable> subscribe_core(std::unique_ptr<abs_observer<T>> observer) override {
        auto ptr = std::make_unique<skip_observer<T>>(std::move(observer), count);
        return source->subscribe(std::move(ptr));
    }

};
#endif //SKIP_H
