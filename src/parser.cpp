
#include "parser.h"

#include <lalr/GrammarCompiler.hpp>
#include <lalr/Parser.ipp>

using namespace oscript;

lalr::ErrorPolicy errp;

ast_node* lalr_gen_ast_node(ast_node* const* data, const lalr::ParserNode<>* nodes, size_t length) {
	ast_node* node = new ast_node();
	node->terminal = false;

	for (alni idx = 0; idx < (alni)length; idx++) {
		if (nodes[idx].symbol()->type == lalr::SYMBOL_NON_TERMINAL) {
			data[idx]->type = nodes[idx].symbol()->identifier;
			data[idx]->type.capture();
			node->childs.PushBack(data[idx]);
		} else {
			ast_node* leave = new ast_node();
			leave->terminal = true;
			leave->type = nodes[idx].symbol()->identifier;
			leave->value = nodes[idx].lexeme().c_str();
			node->childs.PushBack(leave);

			leave->type.capture();
			leave->value.capture();
		}
	}

	return node;
}

parser::parser(const string& grammar) {

	lalr::GrammarCompiler* gcomp = new lalr::GrammarCompiler();

	halni errors = gcomp->compile(grammar.cstr(), grammar.cstr() + strlen(grammar.cstr()), &errp);
	assert(!errors && "invalid grammar given");

	lalr::Parser<const char*, ast_node*>* pars = new lalr::Parser<const char*, ast_node*>(gcomp->parser_state_machine(), &errp);

	pars->set_default_action_handler(lalr_gen_ast_node);
	//pars->parser_action_handlers()("ast_node", );

	grammar_compiler = gcomp;
	lalr_parser = pars;
}

ast_node* parser::parse(const string& oscript) {

	lalr::GrammarCompiler* gcomp = (lalr::GrammarCompiler*) grammar_compiler;
	lalr::Parser<const char*, ast_node*>* pars = (lalr::Parser<const char*, ast_node*>*)lalr_parser;

	bool failed = false;
	try {
		pars->parse(oscript.cstr(), oscript.cstr() + oscript.size());
		failed = !(pars->accepted() && pars->full());
	} catch (...) {
		failed = true;
	}

	return pars->user_data();
}

parser::~parser() {
	delete grammar_compiler;
	delete lalr_parser;
}