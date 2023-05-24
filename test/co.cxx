

#include <coost/co/co.h>
#include <coost/co/defer.h>
#include <iostream>

int main(int argc, char **argv) {
  defer(std::cout << "done" << std::endl);
  go([]() { std::cout << "hello world" << std::endl; });
  coost::co::sleep(1000);
  go([]() { std::cout << "hello world" << std::endl; });
  return 0;
}