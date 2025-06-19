//
// Created by baizeyv on 6/19/2025.
//

#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include <ostream>
#include <utility>

template<typename T, typename... Args>
T* tracked_new(Args&&... args) {
    T* ptr = new T(std::forward<Args>(args)...);
    std::cout << "[tracked new] type = {*" << typeid(T).name() << "*}, ptr = " << static_cast<void*>(ptr) << std::endl;
    return ptr;
}

template<typename T>
void tracked_delete(T* ptr, const char* file, int line) {
    std::cout << "[tracked delete] type = {*" << typeid(T).name() << " -> " << typeid(*ptr).name() << "*}, ptr = " << static_cast<void*>(ptr) << std::endl;
    std::cout << "\tfile: " << file << ", line: " << line << std::endl;
    delete ptr;
}

#define TD(ptr) tracked_delete(ptr, __FILE__, __LINE__)

#endif //UTILS_H
