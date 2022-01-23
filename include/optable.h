#pragma once

#include "vm.h"

enum opslots { 
	OP_NONE,
	OP_CREATE,
	OP_DESTROY,
	OP_CALL,
	OP_SET,
	OP_SAVE,
	OP_LOAD,
};

extern _opearator* object_optable[];

struct  type_method_caller : public object_caller {

	alni len;
	struct Object** in;
	Object** out;
	
	process* proc;

	type_method_caller(Object** in, alni len, Object** out, process*);
	Object* get(alni idx);
	void ret(Object* out);
};
