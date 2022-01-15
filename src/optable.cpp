#pragma once

#include "optable.h"


void create(process* proc, instruction* inst) {
	string* typebame = (string*)inst->operand;
	alni save_adress = inst->args[0];
	proc->cstack[save_adress] = (alni)(NDO->create(*typebame));
}

void destroy(process* proc, instruction* inst) {
	alni obj_adress = inst->args[0];
	NDO->destroy((Object*)proc->cstack[obj_adress]);
}

#include "object/object.h"

struct  type_method_caller : public object_caller {

	alni len;
	Object** in;
	Object** out;

	type_method_caller(Object** in, alni len, Object** out) : len(len), in(in), out(out) {
	}

	Object* get(alni idx) {
		if (idx >= len) {
			throw 0;
		}
		return in[idx];
	}

	void ret(Object* out) {
		*this->out = out;
	}
};

void call(process* proc, instruction* inst) {
	
	alni obj_adress = inst->args[0];
	alni type_method_adress = inst->args[1];
	alni ret_address = inst->args[2];
	alni args_address = inst->args[3];
	alni args_length = inst->args[4];
	
	Object* ob = (Object*)proc->cstack[obj_adress];
	
	type_method_caller caller((Object**)(proc->cstack.mem + args_address), args_length, (Object**)(proc->cstack.mem + ret_address));
	ob->type->methods[type_method_adress](ob, &caller);
}

_opearator* object_optable[] = {
	create,
	destroy,
	call,
};
