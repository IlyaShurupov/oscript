
#include "oscript.h"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    return 1;
  }

  string script = read_file(argv[1]);

  oscript_init();
  virtual_machine vm;

  code bytecode;
  osc_compile(&bytecode, &script);

  vm.run_code(&bytecode, object_optable);

  oscript_finalize();
  return 0;
}