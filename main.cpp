#include <iostream>

#include "memleak.h"
#include "src/rxcxx.h"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
    // ! 内存泄漏检测
    memleak::setup_mem(81920, 0, 0);
    memleak::set_leak_detect(true);
    // ###############################################
    // (rxcxx::observables::range(2, 9) >> rxcxx::operators::take(3)).subscribe([](auto &val) {
    //     std::cout << val << std::endl;
    // });

    // range(2, 9) >> take_while([](const auto& val) -> bool {
    //     if (val % 2 == 0)
    //         return true;
    //     return false;
    // }) >> take(3) >> subscribe([](const auto &val) {
    //     std::cout << val << std::endl;
    // });

    // range(2, 10) >> take_last(3) >> subscribe([](const auto &val) {
    //     std::cout << val << std::endl;
    // });

    // range(2, 10) >> where([](const auto &val) {
    //     if (val % 2 == 0)
    //         return true;
    //     return false;
    // }) >> select([](const auto &val) -> int {
    //     return val * 3;
    // }) >> finally([] {
    //     std::cout << "FFF" << std::endl;
    // }) >> subscribe([](const auto &val) {
    //     std::cout << val << std::endl;
    // });

    range(2, 10) >> skip(2) >> subscribe([](const auto &val) {
        std::cout << val << std::endl;
    });
}
