//
// Created by baizeyv on 6/10/2025.
//

#include "disposed_sentinel.h"

disposed_sentinel * disposed_sentinel::instance() {
    static disposed_sentinel ins;
    return &ins;
}

void disposed_sentinel::dispose() {
}
