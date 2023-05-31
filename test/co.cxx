

#include <coost/co/co.h>
#include <coost/co/defer.h>
#include <coost/log.h>
#include <coost/output.h>
#include <iostream>


int main(int argc, char **argv) {
  defer(coost::println("done"));
  go([]() { coost::println("hello world"); });
  coost::co::sleep(1000);
  go([]() { coost::println("hello world"); });
  for (int i = 0; i < 5; ++i) {
    COOST_ATOMIC_FIRST(3) coost::println("COOST_FIRST", i);
  }
  for (int i = 0; i < 10; ++i) {
    COOST_ATOMIC_EVERY(3) coost::println("COOST_EVERY", i);
  }
  return 0;
}