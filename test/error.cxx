#include <coost/error.h>
#include <iostream>
#include <string>
#include <vector>

int main() {
    coost::error() = ETIMEDOUT;
    std::cout << coost::strerror() << std::endl;
    return 0;
}