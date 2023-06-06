#include "coost/god.h"
#include <coost/rand.h>
#include <iostream>
#include <string>
#include <vector>

int main() {
  // std::cout << coost::god::align_down<4>(121) << std::endl;
  // std::cout << coost::rand() << std::endl;
  // std::cout << coost::randstr() << std::endl;
  // std::vector<uint8_t> v {
  //   0x2b, 0x48, 0x07, 0x01, 0x60, 0xe9, 0xd2
  // };
  // std::vector<uint8_t> v {
  //   0x2b, 0x48, 0x07, 0x01, 0xff, 0xff, 0xff
  // };
  uint64_t v = coost::rand_u64_id();
  auto short_id = coost::to_short_id(v);
  auto id = coost::to_u64_id(short_id);
  std::cout << std::hex << v << std::endl;
  std::cout << short_id << std::endl;
  std::cout << id << std::endl;
  std::cout << (v == id) << std::endl;
  std::cout << coost::rand_short_id() << std::endl;
  return 0;
}