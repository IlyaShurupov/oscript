
#include "compiler.h"

using namespace osc;

compiler::compiler() : pars(tp::read_file("oscript.grammar")) {}

struct compiler_err : public irep_error {
	virtual void ensure(bool expr, const tp::string& def) const override {
		if (!expr) {
			assert(0 && "compiler error");
			printf(def.cstr());
		}
	}
};

void compiler::compile(const tp::string& oscript, fbody* out) {
	assert(out);

	ast_node* ast = pars.parse(oscript);

	if (!ast) {
		return;
	}

	main_node.read(*ast, compiler_err());
	//main_node.evaluate(out);
}

compiler::~compiler() {}