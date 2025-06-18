#include <iostream>

// #include "src/memleak.h"
#include "src/reactive_variable.hpp"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
    // ! 内存泄漏检测
    // memleak::setup_mem(8192, 0, 0);
    // memleak::set_leak_detect(true);
    // ###############################################

    int a = 0;
    reactive_variable<int> test(a);
    // reactive_variable<int> test(5);
    const auto ptr = test.subscribe([](const int& val) {
        std::cout << val << " ??" << std::endl;
    });
    const auto ptr2 = test.subscribe([](const int& val) {
        std::cout << val << " !!" << std::endl;
    });
    test = 5;
    test = 5;
    test = 7;
    //ptr->dispose();
    test.dispose(); // # 在这个dispose后,ptr指向的对象会被delete,之后不能再使用ptr了,悬空指针了
    // test = 4;
    // ptr->dispose();

    // test = 4;
    // test = 5;
    // ptr2->dispose();
}