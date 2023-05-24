#include <coost/god.h>
#include <iostream>
#include <string>
#include <vector>

DEF_has_method(c_str);

int main() {
    std::cout << coost::god::has_method_c_str<std::string>() << std::endl;
    std::cout << coost::god::align_up(123, 64) << std::endl;
    return 0;
}