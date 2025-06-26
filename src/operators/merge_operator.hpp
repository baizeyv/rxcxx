//
// Created by baizeyv on 6/26/2025.
//

#ifndef MERGE_OPERATOR_HPP
#define MERGE_OPERATOR_HPP
#include "../observable.hpp"
#include "../scheduler.h"
#include "../schedules/default_scheduler_impl.h"

namespace rxcxx::operators {
        namespace merge_internal {
            template<typename T, typename OB>
            auto merge(scheduler::creator_func sccr, std::vector<observable<T> > &arr, OB ob) noexcept {
                arr.push_back(ob);
                return [sccr, arr = std::make_shared<std::vector<observable<T> > >(std::move(arr))](auto src) {
                    arr->push_back(src);
                    return observable<>::make_observable<T>([sccr, arr](observer<T> s) {
                        auto sctl = stream_controller<T>(s);
                        auto scdl = sccr();
                        sctl.set_on_finalize([scdl] {
                            scdl.abort();
                        });

                        // prepare subscribers
                        auto subscribers = [sctl, scdl, arr] {
                            auto re = std::vector<observer<T> >();
                            for (auto i = 0; i < arr->size(); i++) {
                                re.push_back(
                                    sctl.template make_observer<T>(
                                        [sctl, scdl](auto, const T &x) {
                                            scdl.schedule([sctl, x] {
                                                sctl.sink_next(x);
                                            });
                                        },
                                        [sctl, scdl](auto, std::exception_ptr err) {
                                            scdl.schedule([sctl, err] {
                                                sctl.sink_error(err);
                                            });
                                        },
                                        [sctl, scdl](auto serial) {
                                            scdl.schedule([sctl, serial] {
                                                sctl.sink_completed(serial);
                                            });
                                        }
                                    )
                                );
                            }
                            return re;
                        }();

                        for (auto i = 0; i < arr->size(); i++) {
                            (*arr)[i].subscribe(subscribers[i]);
                        }
                    });
                };
            }

            template<typename T, typename OB, typename... ARGS>
            auto merge(scheduler::creator_func sccr, std::vector<observable<T> > &arr, OB ob, ARGS... args) {
                arr.push_back(ob);
                return merge(sccr, arr, args...);
            }
        } /* merge_internal */

        template<typename OB, typename... ARGS, std::enable_if_t<is_observable<OB>::value, bool>  = true>
        auto merge(OB ob, ARGS... args) {
            using T = typename OB::value_type;
            std::vector<observable<T> > arr;
            return merge_internal::merge<T>(schedulers::default_scheduler(), arr, ob, args...);
        }

        template<typename OB, typename... ARGS>
        auto merge(scheduler::creator_func sccr, OB ob, ARGS... args) {
            using T = typename OB::value_type;
            std::vector<observable<T> > arr;
            return merge_internal::merge<T>(sccr, arr, ob, args...);
        }
}

#endif //MERGE_OPERATOR_HPP
