//
// Created by baizeyv on 6/20/2025.
//

#ifndef OPERATOR_H
#define OPERATOR_H

class operator_ {
protected:
    bool released = false;
    virtual void release_core() = 0;
public:
    void release() {
        if (released)
            return;
        released = true;
        release_core();
    }
};

#endif //OPERATOR_H
