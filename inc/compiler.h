#pragma once

#include "parser.h"
#include "treecode.h"

// oscript root namespace
namespace oscript {

	struct compiler {
		parser pars;
		treecode main_node;

		compiler();
		void compile(const string& oscript, struct fbody*);
		~compiler();
	};
};