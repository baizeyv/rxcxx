//
// Created by baizeyv on 6/18/2025.
//

#ifndef UNIT_H
#define UNIT_H
#include <iostream>

struct unit {
    bool operator==(const unit &other) const;

    bool operator!=(const unit &other) const;

    bool operator<(const unit &other) const;

    bool operator>(const unit &other) const;
};
std::ostream &operator<<(std::ostream &os, const unit &r);

namespace std {
    template<>
    struct hash<unit> {
        std::size_t operator()(const unit& u) const noexcept {
            return 0;
        }
    };
}
#endif //UNIT_H
