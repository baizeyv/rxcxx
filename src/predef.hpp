//
// Created by baizeyv on 6/21/2025.
//

#ifndef PREDEF_H
#define PREDEF_H
#include <type_traits>

#include "utils.h"
#include "rxcpp/rx-util.hpp"

namespace rxcxx
{

// * ╭───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╮
    
// ! SFINAE 编译时检测类型
struct tag_observer {};

/**
 * * 该类用于在编译时期检测是否是观察者
 * @tparam T 
 */
template<class T>
class is_observer
{
    template<class U>
    static typename U::observer_tag* check(int);
    template<class U>
    static void check(...);
public:
    // # decltype -> 用于在编译时期获取表达式的类型
    static const bool value = std::is_convertible_v<decltype(check<util::decay_t<T>>(0)), tag_observer*>;
};
    
// * ╰───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╯

// * ╭───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╮

    struct tag_dynamic_observer {};

    template<class T>
    class is_dynamic_observer
    {
        struct not_void{};
        template<class U>
        static typename U::dynamic_observer_tag* check(int);
        template<class U>
        static not_void check(...);
    public:
        static const bool value = std::is_convertible_v<decltype(check<util::decay_t<T>>(0)), tag_dynamic_observer*>;
    };
    
// * ╰───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╯
    
/**
 * * 无状态的观察者标签
 */
struct stateless_observer_tag {};

}
#endif //PREDEF_H
