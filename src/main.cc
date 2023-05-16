

#include "co/co.h"
#include <iostream>
DEF_main(argc, argv) {
    go([](){
        std::cout << "hello world" << std::endl;
    });
    co::sleep(100);
}