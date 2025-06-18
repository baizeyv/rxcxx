//
// Created by baizeyv on 6/10/2025.
//

#ifndef SINGLE_ASSIGNMENT_DISPOSABLE_H
#define SINGLE_ASSIGNMENT_DISPOSABLE_H

#include "disposable.h"


/**
 * * 只能赋值一次的终结器
 */
class single_assignment_disposable : public disposable {
private:
    /**
     * * 当前终结器
     */
    disposable* current = nullptr;

    bool is_set = false;

    static void throw_already_assignment();

public:

    void set_disposable(disposable* disposable);

    void dispose() override;
};



#endif //SINGLE_ASSIGNMENT_DISPOSABLE_H
