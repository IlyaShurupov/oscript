

#include "compiler.h"

#include "process.h"

#include "methodobject.h"
#include "primitives/primitives.h"

using namespace osc;
using namespace tp;
using namespace obj;

int main(int argc, char* argv[]) {

  objects_init();
  primitives_define_types();

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

  objects_finalize();
  return 0;
}