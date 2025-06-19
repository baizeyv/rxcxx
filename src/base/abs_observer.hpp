//
// Created by baizeyv on 6/10/2025.
//

#ifndef ABS_OBSERVER_H
#define ABS_OBSERVER_H
#include "disposable.h"
#include "result.h"
#include "scope_guard.h"
#include "single_assignment_disposable.h"
#include "stubs.h"

template<class T>
class abs_observer : public disposable {
private:
    /**
     * * 是否调用过了on_complete
     */
    bool is_called_on_complete = false;

public:
    /**
     * * 订阅来源
     */
    single_assignment_disposable source_subscription;
protected:

    virtual bool auto_dispose_on_complete() {
        return true;
    }

    virtual void on_complete_core(result *rst) = 0;

    virtual void on_next_core(T &p_value) = 0;

    virtual void on_error_core(std::runtime_error &error) = 0;

    virtual void dispose_core() {}

public:
    /**
     * * 是否已被终结
     */
    bool is_disposed = false;

    void on_complete(result *rst) {
        if (is_called_on_complete)
            return;
        is_called_on_complete = true;
        if (is_disposed)
            return;
        bool disposeOnFinally = auto_dispose_on_complete();
        scope_guard sg([this, &disposeOnFinally]() {
            if (disposeOnFinally)
                dispose();
        });
        try {
            on_complete_core(rst);
        } catch (std::runtime_error &e) {
            disposeOnFinally = true;
            stubs::unhandled_exception(e);
        } catch (...) {
        }
    }

    void on_complete_without_result() {
        if (is_called_on_complete)
            return;
        is_called_on_complete = true;
        if (is_disposed)
            return;
        if (auto_dispose_on_complete())
            dispose();
    }

    void on_next(T &p_value) {
        if (is_disposed || is_called_on_complete)
            return;
        try {
            on_next_core(p_value);
        } catch (std::runtime_error &e) {
            on_error(e);
        } catch (...) {
            auto e = std::runtime_error("Unknown Exception!");
            on_error(e);
        }
    }

    void on_error(std::runtime_error &error) {
        if (is_disposed || is_called_on_complete)
            return;
        try {
            on_error_core(error);
        } catch (std::runtime_error &e) {
            stubs::unhandled_exception(e);
        } catch (...) {
            auto e = std::runtime_error("Unknown Exception!");
            stubs::unhandled_exception(e);
        }
    }

    void dispose() override {
        if (is_disposed)
            return;
        is_disposed = true;
        dispose_core();
        source_subscription.dispose();
        delete this;
    }
};
#endif //ABS_OBSERVER_H
