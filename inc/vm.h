#pragma once

#include "list.h"
#include "array.h"




struct virtual_machine {

	list<process*> processes;

	void run_code(code* in, _opearator** optable);
};
