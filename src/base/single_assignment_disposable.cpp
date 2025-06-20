//
// Created by baizeyv on 6/10/2025.
//

#include "single_assignment_disposable.h"

#include <stdexcept>

#include "disposed_sentinel.h"

// ! safe completed

void single_assignment_disposable::throw_already_assignment() {
    throw std::runtime_error("disposable is already assigned.");
}

void single_assignment_disposable::set_disposable(std::unique_ptr<disposable> disposable) {
    if (is_set) {
        throw_already_assignment();
        return;
    }
    is_set = true;
    if (!current) {
        disposable->dispose();
    } else {
        current = std::move(disposable);
    }
}

void single_assignment_disposable::dispose() {
    if (!current)
        return;
    current->dispose();
    current.reset();
}
