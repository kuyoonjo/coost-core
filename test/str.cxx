#include <coost/str.h>
#include <iostream>
#include <string>
#include <vector>

int main() {
    coost::fastring s("abc,ccc");
    auto v = coost::str::split(s, ',');
    std::cout << coost::str::dbg(v) << std::endl;
    return 0;
}