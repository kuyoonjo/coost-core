#include <coost/rand.h>
#include <iostream>
#include <string>
#include <vector>

int main() {
  std::cout << coost::rand() << std::endl;
  std::cout << coost::rand() << std::endl;
  std::cout << coost::rand() << std::endl;
  std::cout << coost::randstr() << std::endl;
  std::cout << coost::randstr() << std::endl;
  std::cout << coost::randstr() << std::endl;
  return 0;
}