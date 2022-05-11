
#include "compiler.h"

using namespace oscript;

compiler::compiler() : pars(read_file("oscript.grammar")) {}

fbody* compiler::compile(const string& oscript) {
	ast_node* ast = pars.parse(oscript);

	if (!ast) {
		return NULL;
	}

	main_node.read(*ast, irep_error());
	return main_node.evaluate();
}

compiler::~compiler() {}