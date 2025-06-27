#include <iostream>

#include "memleak.h"
#include "src/rxcxx.h"
#include "src/extra/rx_event.hpp"
#include "src/extra/rx_message.hpp"
#include "src/extra/specific/int_evt.hpp"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
    // ! 内存泄漏检测
    // memleak::setup_mem(81920, 0, 0);
    // memleak::set_leak_detect(true);
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

    // range(2, 10) >> skip(2) >> subscribe([](const auto &val) {
    //     std::cout << val << std::endl;
    // });

    // const behavior<int> test(2);
    // auto c = test.as_observable() >> publish();
    // c >> subscribe([](const auto &val) {
    //     std::cout << val << std::endl;
    // });
    // test.as_subscriber().on_next(3);
    // test.as_subscriber().on_next(4);
    // test.as_subscriber().on_next(4);
    // test.as_subscriber().on_next(5);
    // auto z = c.connect();

    // replay<int> test(3);
    // auto c = test.as_observable();
    // test.as_subscriber().on_next(20);
    // c >> subscribe([](const auto &val) {
    //     std::cout << val << "A" << std::endl;
    // });
    // test.as_subscriber().on_next(40);
    // c >> subscribe([](const auto &val) {
    //     std::cout << val << "B" << std::endl;
    // });
    // test.as_subscriber().on_next(60);

    // behavior<int> test(3);
    // auto c = test.as_observable();
    // test.as_subscriber().on_next(20);
    //
    // auto afn = [](const auto &val) {
    //     std::cout << val << "A" << std::endl;
    // };
    // c >> subscribe(afn);
    // test.as_subscriber().on_next(40);
    //
    // auto bfn = [](const auto &val) {
    //     std::cout << val << "B" << std::endl;
    // };
    // c >> subscribe(bfn);
    // test.as_subscriber().on_next(60);
    //
    // test.as_subscriber().on_next(80);
    // c >> subscribe([](const auto &val) {
    //     std::cout << val << "C" << std::endl;
    // });
    // c >> tap([](const auto &val) {
    //     std::cout << val << " TAP" << std::endl;
    // }) >> subscribe([](const auto &val) {
    //     std::cout << val << "D" << std::endl;
    // });
    // test.as_subscriber().on_next(100);
    // // 打印当前线程ID
    // auto print_thread = [](const std::string &context) {
    //     std::cout << context << " on thread: "
    //             << std::this_thread::get_id() << std::endl;
    // };
    // print_thread("A");

    // range(1, 10) >>  observe_on(new_thread_scheduler()) >> subscribe([](const auto &val) {
    //     std::cout << val << " TCH " << std::this_thread::get_id() << std::endl;
    // });

    // range(1, 2) >> subscribe_on(new_thread_scheduler()) >> tap([&](int) {
    //     print_thread("B");
    // }) >> observe_on(new_thread_scheduler()) >> tap([&](int) {
    //     print_thread("Z");
    // }) >> subscribe([&](const auto& val) {
    //     print_thread("C");
    // });
    // std::this_thread::sleep_for(std::chrono::seconds(2));

    // native_interval<int>(std::chrono::milliseconds(100), [](const int &val) {
    //             return val >= 20;
    //         }) >> subscribe_on(new_thread_scheduler()) >> tap([&](const auto &val) {
    //             print_thread("C");
    //             std::cout << "ZZ " << val << std::endl;
    //         })
    //         >> operators::blocking() >>
    //         subscribe([&](const auto &val) {
    //             print_thread("B");
    //             std::cout << "<<<<<<<<< " << val << std::endl;
    //         });

    // native_interval<int>(std::chrono::milliseconds(500), [](const int& val) {
    //     return val>=5;
    // }) >> subscribe_on(new_thread_scheduler()) >> tap([&](const auto& val) {
    //     print_thread("Z");
    // }) >> observe_on(new_thread_scheduler()) >> tap([&](const auto& val) {
    //     print_thread("A");
    // }) >> block() >> subscribe([&](const auto& val) {
    //     print_thread("T");
    // });

    // rx_variable<int> test(5);
    // test >> subscribe([](const auto& val) {
    //     std::cout << "V: " << val << std::endl;
    // });
    // test = 3;
    // test = 4;
    // test = 4;
    // test.dispose();
    // test = 4;
    // test = 2;

    // rx_event<int> evt;
    // evt >> subscribe([](const auto& val) {
    //     std::cout << "EVT: " << val << std::endl;
    // });
    // evt = 3;
    // evt = 3;
    // evt.dispose();
    // evt = 5;

    // const rx_message msg;
    // msg >> subscribe([](auto) {
    //     std::cout << "???" << std::endl;
    // });
    // msg();
    // msg();
    // msg();

    // int_msg msg;
    // msg[1] >> subscribe([](auto) {
    //     std::cout << "ZZZZZ" << std::endl;
    // });
    // msg(1);
    // msg(2);
    // msg(1);

    int_evt<int> evt;
    evt[1] >> where([](const auto& val) {
        return val % 2 != 0;
    }) >> subscribe([](const auto& val) {
        std::cout << val << std::endl;
    });
    evt[2] >> subscribe([](const auto& val) {
        std::cout << val << ".." << std::endl;
    });
    evt(1, 2);
    evt(1, 3);
    evt(2, 3);
    evt(1, 5);
}
