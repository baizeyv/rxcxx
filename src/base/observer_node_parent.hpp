//
// Created by baizeyv on 6/11/2025.
//

#ifndef OBSERVER_NODE_PARENT_H
#define OBSERVER_NODE_PARENT_H
#include "observer_node.hpp"

template<class T>
class observer_node_parent {

public:
    observer_node<T> *root = nullptr;

    virtual bool is_completed_or_disposed() const = 0;
};

#endif //OBSERVER_NODE_PARENT_H
