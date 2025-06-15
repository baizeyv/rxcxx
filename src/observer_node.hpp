//
// Created by baizeyv on 6/11/2025.
//

#ifndef OBSERVER_NODE_H
#define OBSERVER_NODE_H
#include "abs_observer.hpp"
#include "disposable.h"

template<class T>
class observer_node<T> final : disposable {
public:
    const abs_observer<T>* observer = nullptr;

    observer_node_parent<T>* parent = nullptr;

    observer_node<T>* previous = nullptr;

    observer_node<T>* next = nullptr;

    observer_node(observer_node_parent<T>* parent, const abs_observer<T>* observer) : observer(observer), parent(parent) {
        if (parent->root == nullptr) {
            // # 设置根节点
            parent->root = this;
        } else {
            observer_node<T>* prev;
            if (parent->root->previous == nullptr) {
                prev = parent->root;
            } else {
                prev = parent->root->previous;
            }
            prev->next = this;
            this->previous = prev;
            parent->root->previous = this;
        }
    }

    void dispose() override {
        // TODO: delete所有new出的对象
        if (parent == nullptr)
            return;

        if (parent->is_completed_or_disposed())
            // # 已经完成或终结了
            return;
        if (parent->root == this) {
            // # 这是根节点
            if (previous == nullptr || next == nullptr) {
                parent->root = nullptr;
            } else {
                observer_node<T>* root = next;
                if (root->next == nullptr)
                    root->previous = nullptr;
                else
                    root->previous = previous;

                parent->root = root;
            }
        } else {
            if (previous != nullptr)
                previous->next = next;
            if (next != nullptr)
                next->previous = previous;
            else
                parent->root->previous = previous;
        }
        parent = nullptr;
    }

};

#endif //OBSERVER_NODE_H
