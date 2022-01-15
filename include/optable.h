#pragma once

#include "vm.h"

enum opslots { 
	OP_CREATE,
	OP_DESTROY,
	OP_CALL,
};

extern _opearator* object_optable[];
