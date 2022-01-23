
#include "oscript.h"


int main(int argc, char* argv[]) {

	if (argc < 2) {
		return 1;
	}

	File entry_script;
	entry_script.open(argv[1], FileOpenFlags::LOAD);

	string script;
	script.alloc(entry_script.size());

	entry_script.read_bytes(script.str, script.len(), 0);

	
	oscript_init();
	virtual_machine vm;

	code bytecode;
	osc_compile(&bytecode, &script);

	vm.run_code(&bytecode, object_optable);

	return 0;
}