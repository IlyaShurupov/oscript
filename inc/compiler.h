#pragma once

#include "parser.h"
#include "treecode.h"

namespace osc {

	struct compiler {
		parser pars;
		treecode main_node;

		compiler();
		void compile(const tp::string& oscript, struct fbody*);
		~compiler();
	};
};