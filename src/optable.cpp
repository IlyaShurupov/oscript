#pragma once

#include "optable.h"

#include "object/object.h"

#include "primitives/stringobject.h"

void create(process* proc, instruction* inst) {
	string* typebame = (string*)inst->operand;
	alni save_adress = inst->args[0];
	proc->cstack[save_adress] = (alni)(NDO->create(*typebame));
}

void destroy(process* proc, instruction* inst) {
	alni obj_adress = inst->args[0];
	NDO->destroy((Object*)proc->cstack[obj_adress]);
}

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
	alni ret_address = inst->args[1];
	alni args_address = inst->args[2];
	alni args_length = inst->args[3];
	type_method_adress* tm_adress = (type_method_adress*)inst->operand;
	
	Object* ob = (Object*)proc->cstack[obj_adress];
	
	type_method_caller caller((Object**)(proc->cstack.mem + args_address), args_length, (Object**)(proc->cstack.mem + ret_address));
	tm_adress(ob, &caller);
}

void none_op(process* proc, instruction* inst) {}

void set(process* proc, instruction* inst) {
	alni object_adress = inst->args[0];
	alni val_type = inst->args[1];
	void* val_p = (void*)inst->operand;

	Object* ob = (Object*)proc->cstack[object_adress];
	
	switch (val_type) {

		case 0: { // int
			NDO->set(ob, *(alni*)val_p);
			break;
		}
		
		case 1: { // float
			NDO->set(ob, *(float*)val_p);
			break;
		}

		case 2: { // string
			NDO->set(ob, *(string*)val_p);
			break;
		}
	}
}

void save(process* proc, instruction* inst) {
	alni obj_adress = inst->args[0];
	alni string_path_obj_adress = inst->args[1];

	Object* ob = (Object*)proc->cstack[obj_adress];
	StringObject* path = (StringObject*)proc->cstack[string_path_obj_adress];

	NDO->save(ob, path->val);
}

void load(process* proc, instruction* inst) {
	alni ret_adress = inst->args[0];
	alni string_path_obj_adress = inst->args[1];

	StringObject* path = (StringObject*)proc->cstack[string_path_obj_adress];

	proc->cstack[ret_adress] = (alni)NDO->load(path->val);
}

_opearator* object_optable[] = {
	none_op,
	create,
	destroy,
	call,
	set,
	save,
	load,
};
