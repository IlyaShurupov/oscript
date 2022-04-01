
#include "vm.h"


void virtual_machine::run_code(code* in, _opearator** optable) {
	process proc(in, optable);

	while (!proc.finished) {
		proc.run_instruction();
	}
}
