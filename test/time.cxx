#include <coost/time.h>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::cout << coost::time::now::str() << std::endl;
    coost::time::sleep::ms(1000);
    std::cout << coost::time::now::str() << std::endl;
    return 0;
}