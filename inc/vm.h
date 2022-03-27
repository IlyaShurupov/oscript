#pragma once

#include "list.h"
#include "array.h"

#define CALL_STACK_SIZE 100

typedef void(_opearator)(struct process*, struct instruction* inst);

struct instruction {

	uint1 opslot;
	uint1 args[7];
	alni operand = 0;
};

struct code : Array<instruction> {};


struct call_stack {
	alni mem[CALL_STACK_SIZE];

	alni bp = 0;
	alni fp = 1;
	
	call_stack() {
		memsetv(&mem, CALL_STACK_SIZE * sizeof(alni), 0);
	}

	alni& operator[](alni idx) {
		return mem[bp + idx];
	}

};


struct process {

	_opearator** optable;

	bool finished = false;
	alni finish_state = 0;

	call_stack cstack;

	code* fbody;
	alni ip = 0;

	process(code* entry, _opearator** optable);

	void run_instruction();

};


struct virtual_machine {

	list<process*> processes;

	void run_code(code* in, _opearator** optable);
};
