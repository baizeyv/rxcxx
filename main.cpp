#include <iostream>

#include "src/memleak.h"
// #include "src/event/int_message.h"
// #include "src/event/simple_event.hpp"
// #include "src/event/simple_message.h"
// #include "src/event/string_message.h"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
    // ! 内存泄漏检测
    memleak::setup_mem(81920, 0, 0);
    memleak::set_leak_detect(true);
    // ###############################################
}