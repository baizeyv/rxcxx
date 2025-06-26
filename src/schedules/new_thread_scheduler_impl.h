//
// Created by baizeyv on 6/25/2025.
//

#ifndef NEW_THREAD_SCHEDULER_IMPL_H
#define NEW_THREAD_SCHEDULER_IMPL_H
#include <thread>

#include "../scheduler.h"
#include "../scheduler_interface.h"


namespace rxcxx::schedulers {
    class new_thread_scheduler_impl : public scheduler_interface {
    private:
        std::thread thread;

    public:
        new_thread_scheduler_impl() noexcept;

        ~new_thread_scheduler_impl() noexcept override;

        void run(call_in_context_func call_in_context) noexcept override;

        void schedule(const function_type &f) noexcept override;

        void detach() noexcept override;
    };

    inline auto new_thread_scheduler() {
        return []() {
            return scheduler(std::make_shared<new_thread_scheduler_impl>());
        };
    }

    inline auto observe_on_new_thread() {
        return new_thread_scheduler();
    }
}


#endif //NEW_THREAD_SCHEDULER_IMPL_H
