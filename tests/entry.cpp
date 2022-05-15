

#include "compiler.h"

#include "process.h"

#include "methodobject.h"

using namespace oscript;

int main(int argc, char* argv[]) {

  NDO->define(&MethodObjectType);
  NDO->add_sl_callbacks(&slcb_script_section);

  if (argc < 2) {
    return 1;
  }

  compiler cp;
  process proc;
  cp.compile(read_file(argv[1]), proc.fb);

  proc.run();

  delete proc.fb;
  return 0;
}