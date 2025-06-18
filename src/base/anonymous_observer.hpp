//
// Created by baizeyv on 6/17/2025.
//

#ifndef ANONYMOUS_OBSERVER_H
#define ANONYMOUS_OBSERVER_H
#include "abs_observer.hpp"

template<class T>
class anonymous_observer final : public abs_observer<T> {
private:
    std::function<void(T &)> on_next;
    std::function<void(std::runtime_error &)> on_error;
    std::function<void(result *)> on_complete;

public:
    anonymous_observer(const std::function<void(T &)> on_next,
                       const std::function<void(std::runtime_error &)> &on_error,
                       const std::function<void(result *)> &on_complete) : on_next(on_next), on_error(on_error),
                                                                           on_complete(on_complete) {
    }

protected:
    void on_complete_core(result *rst) override {
        if (on_complete)
            on_complete(rst);
    };

    void on_next_core(T &p_value) override {
        if (on_next)
            on_next(p_value);
    };

    void on_error_core(std::runtime_error &error) override {
        if (on_error)
            on_error(error);
    };
};
#endif //ANONYMOUS_OBSERVER_H
