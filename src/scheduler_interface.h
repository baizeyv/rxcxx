//
// Created by baizeyv on 6/25/2025.
//

#ifndef SCHEDULER_INTERFACE_H
#define SCHEDULER_INTERFACE_H
#include <functional>


namespace rxcxx {
    class scheduler_interface {
    public:
        /**
         * * 调度类型
         */
        enum class schedule_type {
            direct,
            queuing
        };

        using function_type = std::function<void()>;
        using call_in_context_func = std::function<void()>;

    private:
        schedule_type sc_type;

    protected:
        explicit scheduler_interface(schedule_type p_type) noexcept;

    public:
        virtual ~scheduler_interface() = default;

        schedule_type get_schedule_type() const noexcept;

        virtual void run(call_in_context_func call_in_context) noexcept = 0;

        virtual void schedule(const function_type &f) noexcept = 0;

        virtual void detach() noexcept = 0;

    };
}



#endif //SCHEDULER_INTERFACE_H
