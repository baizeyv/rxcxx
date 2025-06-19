#include <iostream>

#include "src/memleak.h"
#include "src/event/int_event.hpp"
#include "src/event/int_message.h"
#include "src/event/simple_event.hpp"
#include "src/event/simple_message.h"
#include "src/variable/reactive_variable.hpp"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
    // ! 内存泄漏检测
    memleak::setup_mem(8192, 0, 0);
    memleak::set_leak_detect(true);
    // ###############################################

    /*
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

    simple_event<int> evt;
    const auto evtPtr = evt.subscribe([](const int& val) {
        std::cout << val << " ##" << std::endl;
    });
    int xx = 9;
    evtPtr->dispose();
    evt.fire(xx);

    simple_message msg;
    const auto msgPtr = msg.subscribe([](const unit& val) {
        std::cout << val << " ##" << std::endl;
    });
    msg.fire();
    msg.fire();
    msg.fire();
    */

    // int_event<int> iv;
    // const auto ivPtr = iv.subscribe(1, [](const int& val) {
    //     std::cout << val << " &&" << std::endl;
    // });
    // iv.fire(1, 9);
    // iv.fire(1, 8);

    int_message im;
    const auto imPtr = im.subscribe(1, [](const unit& _) {
        std::cout << "ZZZZ" << std::endl;
    });
    im.fire(2);
    im.fire(1);
}