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
