//
// Created by baizeyv on 6/25/2025.
//

#ifndef RXCXX_H
#define RXCXX_H

#include "rxcxx-utils.h"

#include "observables/empty_observable.hpp"
#include "observables/range_observable.hpp"
#include "observables/error_observable.hpp"
#include "observables/just_observable.hpp"
#include "observables/never_observable.hpp"
#include "observables/iterate_observable.hpp"
#include "observables/defer_observable.hpp"
#include "observables/native_interval_observable.hpp"
#include "observables/blocking_observables.hpp"
// * ╭───────────╮
// * │ OPERATORS │
#include "operators/subscribe_operator.hpp"
#include "operators/take_operator.hpp"
#include "operators/take_until_operator.hpp"
#include "operators/take_while_operator.hpp"
#include "operators/take_last_operator.hpp"
#include "operators/last_operator.hpp"
#include "operators/first_operator.hpp"
#include "operators/where_operator.hpp"
#include "operators/select_operator.hpp"
#include "operators/select_many_operator.hpp"
#include "operators/finally_operator.hpp"
#include "operators/skip_operator.hpp"
#include "operators/skip_until_operator.hpp"
#include "operators/skip_while_operator.hpp"
#include "operators/distinct_until_changed_operator.hpp"
#include "operators/on_error_resume_next_operator.hpp"
#include "operators/publish_operator.hpp"
#include "operators/tap_operator.hpp"
#include "operators/subscribe_on_operator.hpp"
#include "operators/observe_on_operator.hpp"
#include "operators/retry_operator.hpp"
#include "operators/native_delay_operator.hpp"
#include "operators/merge_operator.hpp"
#include "operators/amb_operator.hpp"
#include "operators/blocking_operator.hpp"
#include "operators/timeout_operator.hpp"
#include "operators/zip_operator.hpp"
// * ╰───────────╯

#include "schedules/default_scheduler_impl.h"
#include "schedules/new_thread_scheduler_impl.h"

#include "subjects/subject.hpp"
#include "subjects/behavior.hpp"
#include "subjects/replay.hpp"

#include "extra/rx_variable.hpp"
#include "extra/msg_set.hpp"
#include "extra/specific/int_msg.hpp"
#include "extra/specific/string_msg.hpp"
#include "extra/specific/int_evt.hpp"
#include "extra/specific/string_msg.hpp"

namespace rx {
    using namespace rxcxx;
    using namespace rxcxx::schedulers;
    using namespace rxcxx::observables;
    using namespace rxcxx::operators;
    using namespace rxcxx::subjects;
    using namespace rxcxx::extra;
    using namespace rxcxx::extra::specific;
}
using namespace rx;
#endif //RXCXX_H
