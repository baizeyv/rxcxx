//
// Created by baizeyv on 6/18/2025.
//

#include "unit.h"


bool unit::operator==(const unit &other) const {
    return true;
}

bool unit::operator!=(const unit &other) const {
    return false;
}

bool unit::operator<(const unit &other) const {
    return false;
}

bool unit::operator>(const unit &other) const {
    return false;
}


std::ostream &operator<<(std::ostream &os, const unit &r) {
    os << "(UNIT)";
    return os;
}
