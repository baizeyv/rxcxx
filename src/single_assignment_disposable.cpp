//
// Created by baizeyv on 6/10/2025.
//

#include "single_assignment_disposable.h"

#include <iostream>
#include <stdexcept>

#include "disposed_sentinel.h"

void single_assignment_disposable::throw_already_assignment() {
    throw std::runtime_error("disposable is already assigned.");
}

void single_assignment_disposable::set_disposable(disposable *disposable) {
    // ! 这里传入的disposable指针是new出来的,需要在合适的时候delete
    if (is_set) {
        throw_already_assignment();
        return;
    }
    is_set = true;
    if (current == disposed_sentinel::instance()) {
        disposable->dispose();
        // ! dispose中调用了delete,已经delete了这个disposable了
    } else {
        current = disposable;
    }
}

void single_assignment_disposable::dispose() {
    if (current == disposed_sentinel::instance() || current == nullptr) {
        return;
    }
    current->dispose();
    // ! dispose中调用了delete,已经delete了这个current了
    current = disposed_sentinel::instance();
}
