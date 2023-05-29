#include "coost/fastream.h"
#include "coost/fastring.h"
#include <coost/output.h>
#include <iostream>
#include <string>
#include <vector>

int main() {
    coost::output(stdout, "output");
    coost::println("println");
    coost::eprintln("eprintln");
    return 0;
}