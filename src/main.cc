

#include "co/co.h"
#include <iostream>
int main(int argc, char** argv) {
    co::go([](){
        std::cout << "hello world" << std::endl;
    });
    co::sleep(1000);
    co::go([](){
        std::cout << "hello world" << std::endl;
    });
    co::sleep(100);
    return 0;
}