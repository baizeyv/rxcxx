//
// Created by baizeyv on 6/19/2025.
//

#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include <ostream>
#include <utility>

namespace tracked {
    inline bool enabled = false;
}

template<typename T, typename... Args>
T* tracked_new(const char* file, int line, Args&&... args) {
    T* ptr = new T(std::forward<Args>(args)...);
    if (tracked::enabled) {
        std::cout << "[tracked new] type = {*" << typeid(T).name() << "*}, ptr = " << static_cast<void*>(ptr) << std::endl;
        std::cout << "\tfile: " << file << ", line: " << line << std::endl;
    }
    return ptr;
}

template<typename T>
void tracked_delete(T* ptr, const char* file, int line) {
    if (tracked::enabled) {
        std::cout << "[tracked delete] type = {*" << typeid(T).name() << " -> " << typeid(*ptr).name() << "*}, ptr = " << static_cast<void*>(ptr) << std::endl;
        std::cout << "\tfile: " << file << ", line: " << line << std::endl;
    }
    delete ptr;
}

#define TN(T, ...) tracked_new<T>(__FILE__, __LINE__, ##__VA_ARGS__)
#define TD(ptr) tracked_delete(ptr, __FILE__, __LINE__)

namespace rxcxx
{
    namespace util
    {
        template<class T>
        using decay_t = typename std::decay<T>::type;

        using error_ptr = std::exception_ptr;
        
        // If called during exception handling, return the currently caught exception.
        // Otherwise return null.
        inline error_ptr current_exception() {
            return std::current_exception();
        }
    }
}

#endif //UTILS_H
