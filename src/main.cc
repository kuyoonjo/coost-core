

#include "co/co.h"
#include <iostream>
int _co_main(int argc, char** argv);
int main(int argc, char** argv) {
    // flag::parse(argc, argv);
    int r;
    co::wait_group wg(1);
    go([&](){
        r = _co_main(argc, argv);
        wg.done();
    });
    wg.wait();
    return r;
} \
int _co_main(int argc, char** argv) {
    go([](){
        std::cout << "hello world" << std::endl;
    });
    co::sleep(1000);
    go([](){
        std::cout << "hello world" << std::endl;
    });
    co::sleep(100);
    return 0;
}