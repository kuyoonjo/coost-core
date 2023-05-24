#include <coost/crypto/murmur_hash.h>
#include <initializer_list>
#include <iostream>
#include <string>
#include <vector>

struct a {
    int x;
    int y;
};
int main() {
    uint32_t i = 0x01020308;
    auto hash = coost::crypto::murmur_hash(&i, sizeof(uint32_t));
    std::cout << hash << std::endl;
    return 0;
}