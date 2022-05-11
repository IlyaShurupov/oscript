#pragma once

#include "parser.h"
#include "treecode.h"

// oscript root namespace
namespace oscript {

	struct compiler {
		parser pars;
		treecode main_node;

		compiler();
		fbody* compile(const string& oscript);
		~compiler();
	};
};