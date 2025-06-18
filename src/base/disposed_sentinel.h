//
// Created by baizeyv on 6/10/2025.
//

#ifndef DISPOSED_SENTINEL_H
#define DISPOSED_SENTINEL_H
#include "disposable.h"


/**
 * * 终结器哨兵
 */
class disposed_sentinel : public disposable{
private:
    disposed_sentinel() = default;
    ~disposed_sentinel() = default;
public:
    static disposed_sentinel* instance();

    /**
     * * 禁止移动
     */
    disposed_sentinel(const disposed_sentinel&) = delete;

    /**
     * * 禁止赋值
     * @return
     */
    disposed_sentinel& operator=(const disposed_sentinel&) = delete;

    void dispose() override;
};



#endif //DISPOSED_SENTINEL_H
