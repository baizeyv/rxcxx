#include <iostream>

#include "src/reactive_variable.hpp"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
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
    test.dispose();
    // test = 4;
    ptr->dispose();

    // test = 4;
    // test = 5;
    // ptr2->dispose();
}