//
// Created by baizeyv on 6/11/2025.
//

#ifndef SCOPE_GUARD_H
#define SCOPE_GUARD_H
#include <functional>


class scope_guard {
public:
    scope_guard(std::function<void()> f);
    ~scope_guard();
private:
    std::function<void()> finally_func;
};



#endif //SCOPE_GUARD_H
