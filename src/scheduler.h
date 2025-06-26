//
// Created by baizeyv on 6/25/2025.
//

#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

#include "scheduler_interface.h"


namespace rxcxx {
    class scheduler {
    public:
        using function_type = typename scheduler_interface::function_type;
        using creator_func = std::function<scheduler()>;

    private:
        using interface_ptr = std::shared_ptr<scheduler_interface>;

        struct member {
            interface_ptr interface;
            std::mutex mtx;
            std::condition_variable cond;
            std::queue<function_type> queue;
            bool abort = false;
        };

        std::shared_ptr<member> mem;

    public:
        template<typename ISP>
        explicit scheduler(ISP isp) noexcept : mem(std::make_shared<member>()) {
            mem->interface = std::dynamic_pointer_cast<scheduler_interface>(isp);
            if (mem->interface->get_schedule_type() == scheduler_interface::schedule_type::queuing) {
                auto m = mem;
                mem->interface->run([m]() {
                    while (true) {
                        auto q = [m]() -> std::queue<function_type> {
                            std::unique_lock<std::mutex> lock(m->mtx);
                            m->cond.wait(lock, [m] {
                                return !m->queue.empty() || m->abort;
                            });
                            if (m->abort) {
                                return std::queue<function_type>();
                            }
                            return std::move(m->queue);
                        }();
                        if (q.empty())
                            break;
                        while (!q.empty()) {
                            m->interface->schedule(q.front());
                            q.pop();
                        }
                    }
                });
            } else {
                mem->interface->run({});
            }
        }

        scheduler(const scheduler & src) noexcept;

        scheduler& operator = (const scheduler& src) noexcept;

        virtual ~scheduler() noexcept;

        template<typename F>
        void schedule(F &&f) const noexcept {
            if (mem->interface->get_schedule_type() == scheduler_interface::schedule_type::queuing) {
                std::unique_lock<std::mutex> lock(mem->mtx);
                mem->queue.push(std::forward<F>(f));
                lock.unlock();
                mem->cond.notify_one();
            } else {
                mem->interface->schedule(std::forward<F>(f));
            }
        }

        void abort() const noexcept;
    };
}

#endif //SCHEDULER_H
