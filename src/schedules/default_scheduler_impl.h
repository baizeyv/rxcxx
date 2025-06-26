//
// Created by baizeyv on 6/25/2025.
//

#ifndef DEFAULT_SCHEDULER_IMPL_H
#define DEFAULT_SCHEDULER_IMPL_H
#include "../scheduler.h"
#include "../scheduler_interface.h"


namespace rxcxx::schedulers {

    class default_scheduler_impl : public scheduler_interface{
    public:
        default_scheduler_impl() noexcept;

        ~default_scheduler_impl() override;

        void run(call_in_context_func call_in_context) noexcept override;

        void schedule(const function_type &f) noexcept override;

        void detach() noexcept override;
    };

    inline auto default_scheduler() noexcept {
        return [] {
            return scheduler(std::make_shared<default_scheduler_impl>());
        };
    }

}


#endif //DEFAULT_SCHEDULER_IMPL_H
