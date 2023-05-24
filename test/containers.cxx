#include <coost/containers/array.h>
#include <coost/containers/table.h>
#include <coost/containers/map.h>
#include <iostream>

struct a {
    int x;
    int y;
};
int main() {
    coost::array<int> a(4, 3);
    std::cout << a[0] << std::endl;
    std::cout << a[1] << std::endl;
    coost::table<int> t(1,1);
    t[0] = 1;
    t[3] = 2;
    std::cout << t[0] << std::endl;
    std::cout << t[3] << std::endl;
    coost::map<int, int> m;
    m.emplace(1, 1);
    m.emplace(2, 2);
    std::cout << m[1] << std::endl;
    std::cout << m[2] << std::endl;

    return 0;
}