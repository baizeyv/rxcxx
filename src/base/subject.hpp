//
// Created by baizeyv on 6/11/2025.
//

#ifndef SUBJECT_H
#define SUBJECT_H
#include <stdexcept>

#include "result.h"

/**
 * * 主题接口
 * * 实现这个类需要继承自 observable<T>
 * @tparam T
 */
template<class T>
class subject {
public:
    virtual void on_next(T& value) = 0;
    virtual void on_error(std::runtime_error &error) = 0;
    virtual void on_complete(result* rst) = 0;
};

#endif //SUBJECT_H
