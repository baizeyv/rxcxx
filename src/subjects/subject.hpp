//
// Created by baizeyv on 6/26/2025.
//

#ifndef SUBJECT_HPP
#define SUBJECT_HPP
#include "../observables/connectable_observable.hpp"
#include "../observables/error_observable.hpp"
#include "../operators/on_error_resume_next_operator.hpp"
#include "../operators/publish_operator.hpp"

namespace rxcxx::subjects {
    template<class T>
    class subject {
    public:
        using value_type = T;
        using source_type = observables::connectable<value_type>;
        using observer_type = observer<value_type>;
        using subscriber_type = observer<value_type>;

    private:
        struct member {
            source_type source;
            subscriber_type subscriber;
            std::exception_ptr error = nullptr;
            subscription subscription;
            std::recursive_mutex mtx;
            std::vector<stream_controller<value_type> > sctls;

            ~member() {
                subscription.dispose();
                std::lock_guard<std::recursive_mutex> lock(mtx);
                std::for_each(sctls.begin(), sctls.end(), [](auto &sctl) {
                    sctl.finalize();
                });
            }
        };

        std::shared_ptr<member> mem;

    protected:
        bool completed() const noexcept {
            return !mem->subscription.is_no_disposed();
        }

        std::exception_ptr error() const noexcept { return mem->error; }

    public:
        subject() noexcept : mem(std::make_shared<member>()) {
            std::weak_ptr<member> m = mem;
            mem->source = observable<>::make_observable<value_type>([m](subscriber_type s) {
                              if (auto mm = m.lock()) mm->subscriber = s;
                          })
                          >> operators::on_error_resume_next([m](std::exception_ptr err) {
                              if (auto mm = m.lock()) mm->error = err;
                              return observables::error<value_type>(err);
                          })
                          >> operators::publish();
            mem->subscription = mem->source.connect();
        }

        virtual ~subject() = default;

        auto as_subscriber() const noexcept {
            return mem->subscriber;
        }

        virtual observable<T> as_observable() const noexcept {
            auto m = mem;
            return observable<>::make_observable<value_type>(
                [m](subscriber_type s) {
                    auto sctl = stream_controller<value_type>(s); {
                        std::lock_guard<std::recursive_mutex> lock(m->mtx);
                        m->sctls.push_back(sctl);
                    }
                    sctl.set_on_finalize([m] {
                    });
                    m->source.subscribe(sctl.template make_observer<value_type>(
                        [sctl](auto, const value_type &x) {
                            sctl.sink_next(x);
                        },
                        [sctl](auto, std::exception_ptr err) {
                            sctl.sink_error(err);
                        },
                        [sctl](auto serial) {
                            sctl.sink_completed(serial);
                        }
                    ));
                    if (m->error) {
                        sctl.sink_error(m->error);
                    } else if (!m->subscription.is_no_disposed()) {
                        sctl.sink_completed_force();
                    }
                }
            );
        }
    };
}

#endif //SUBJECT_HPP
