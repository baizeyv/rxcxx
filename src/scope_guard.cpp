//
// Created by baizeyv on 6/11/2025.
//

#include "scope_guard.h"

#include <utility>

scope_guard::scope_guard(std::function<void()> f) : finally_func(std::move(f)) {
}

scope_guard::~scope_guard() {
    finally_func();
}
