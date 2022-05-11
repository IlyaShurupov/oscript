

#include "compiler.h"

#include "process.h"

using namespace oscript;

int main(int argc, char* argv[]) {

  if (argc < 2) {
    return 1;
  }

  compiler cp;
  process proc;
  proc.fb = cp.compile(read_file(argv[1]));

  proc.run();

  delete proc.fb;
  return 0;
}