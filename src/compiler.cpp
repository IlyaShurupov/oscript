
#include "compiler.h"

using namespace oscript;

compiler::compiler() : pars(read_file("oscript.grammar")) {}

void compiler::compile(const string& oscript, fbody* out) {
	assert(out);

	ast_node* ast = pars.parse(oscript);

	if (!ast) {
		return;
	}

	main_node.read(*ast, irep_error());
	main_node.evaluate(out);
}

compiler::~compiler() {}