

#include "co/co.h"
DEF_main(argc, argv) {
    go([](){
        LOG << "hello world";
    });
    co::sleep(100);
}