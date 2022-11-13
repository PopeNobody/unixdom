#include <errno.h>
#include <string>
#include <string.h>
#include <iostream>
#include <vector>
#include <array>
#include "errno.hh"

using namespace std;
using namespace err;

int main(int, char**){
  auto &list=errno_t::errs();
  return 0;
};
