#include "src/rxcxx.h"
#include "src/extra/reactive_variable.hpp"
#include "src/extra/simple_event.hpp"
#include "src/extra/simple_message.h"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
    /*
    range(0, 10) | take(5) | subscribe<int>([](const int &val) {
        std::cout << val << " ../" << std::endl;
    });
    const auto a = range(2, 80) >> skip(40) >> take(2) >> subscribe<int>([](const int &val) {
                                                                             std::cout << val << " ???" << std::endl;
                                                                         }, []() {
                                                                             std::cout << "compl";
                                                                         });
    a.unsubscribe();
    */
    // subjects::behavior<int> test(2);
    // test.get_observable() >> subscribe<int>([](const int& val) {
    //     std::cout << val << " >" << std::endl;
    // });
    // test.get_observable() >> subscribe<int>([](const int& val) {
    //     std::cout << val << " !" << std::endl;
    // });
    // test.get_subscriber().on_next(2);

    // reactive_variable<int> tt(5);
    // tt >> subscribe<int>([](const int& val) {
    //     std::cout << val << " !!!" << std::endl;
    // });
    // tt >> take(3) >> subscribe<int>([](const int& val) {
    //     std::cout << val << " ???" << std::endl;
    // });
    // tt = 9;
    // tt = 10;
    // tt = 11;
    // tt = 12;

    // simple_event<int> test;
    // auto c = test >> subscribe<int>([](const int& val) {
    //     std::cout << val << " &&" << std::endl;
    // });
    // test.fire(1);
    // test.fire(9);
    // c.unsubscribe();
    // test.fire(9);

    simple_message test;
    auto b = test >> subscribe<unit>([](unit) {
        std::cout << "FIRE" << std::endl;
    });
    test.fire();
    b.unsubscribe();
    test.fire();
}
