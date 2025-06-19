//
// Created by baizeyv on 6/10/2025.
//

#ifndef DISPOSABLE_H
#define DISPOSABLE_H

/**
 * * 终结器接口
 */
class disposable {
public:
    virtual ~disposable() = default;

    virtual void dispose() = 0;
};

#endif //DISPOSABLE_H
