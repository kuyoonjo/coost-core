#include <coost/atomic.h>
#include <iostream>
#include <string>
#include <vector>

int main() {
    int i = 0;
    coost::atomic_inc(&i);
    std::cout << i << std::endl;
    return 0;
}