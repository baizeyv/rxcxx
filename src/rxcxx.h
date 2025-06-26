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
#include "operators/finally_operator.hpp"
#include "operators/skip_operator.hpp"
#include "operators/skip_while_operator.hpp"
#include "operators/distinct_until_changed_operator.hpp"
// * ╰───────────╯

#include "schedules/default_scheduler_impl.h"
#include "schedules/new_thread_scheduler_impl.h"

namespace rx {
    using namespace rxcxx;
    using namespace rxcxx::schedulers;
    using namespace rxcxx::observables;
    using namespace rxcxx::operators;
}
using namespace rx;
#endif //RXCXX_H
