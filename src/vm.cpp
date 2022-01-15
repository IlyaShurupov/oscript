
#include "vm.h"

process::process(code* entry, _opearator** optable) {
	this->optable = optable;
	fbody = entry;
}

void process::run_instruction() {

	if (fbody->Len() <= ip) {
		finished = true;
		finish_state = 1;
		return;
	}

	instruction& inst = (*fbody)[ip];
	optable[inst.opslot](this, &inst);

	ip++;
}


void virtual_machine::run_code(code* in, _opearator** optable) {
	process proc(in, optable);

	while (!proc.finished) {
		proc.run_instruction();
	}
}
