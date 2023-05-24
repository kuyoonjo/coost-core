#include "coost/clist.h"
#include <coost/mem.h>
#include <initializer_list>
#include <iostream>
#include <string>
#include <vector>

struct a {
    int x;
    int y;
};
int main() {
    auto v = coost::mem::make<a>();
    v->x = 1;
    v->y = 2;
    std::cout << v->x << std::endl;
    std::cout << v->y << std::endl;
    coost::mem::del(v);
    return 0;
}