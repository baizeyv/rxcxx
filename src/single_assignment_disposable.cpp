//
// Created by baizeyv on 6/10/2025.
//

#include "single_assignment_disposable.h"

#include <stdexcept>

#include "disposed_sentinel.h"

void single_assignment_disposable::throw_already_assignment() {
    throw std::runtime_error("disposable is already assigned.");
}

void single_assignment_disposable::set_disposable(disposable *disposable) {
    if (is_set) {
        throw_already_assignment();
        return;
    }
    is_set = true;
    if (current == disposed_sentinel::instance()) {
        disposable->dispose();
        delete disposable;
    } else {
        current = disposable;
    }
}

void single_assignment_disposable::dispose() {
    if (current == disposed_sentinel::instance() || current == nullptr) {
        return;
    }
    current->dispose();
    delete current;
    current = disposed_sentinel::instance();
}
