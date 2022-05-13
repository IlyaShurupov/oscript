
#include "treecode.h"

using namespace oscript;

void irep_error::ensure(bool expr, const string& def) const {
	if (!expr) {
		rterror((string("oscript parser error - ") + def).cstr());
	}
}


void create_expr_node::read(const ast_node& node, const irep_error& err) {
	type.id = node.childs[0]->value;
	name.id = node.childs[1]->value;
}

void destroy_expr_node::read(const ast_node& node, const irep_error& err) {
	del_expr = new expr_node();
	del_expr->read(*node.childs[1], err);
}

destroy_expr_node::~destroy_expr_node() {
	if (del_expr) {
		delete del_expr;
	}
}

void expr_node::clear() {
	switch (tp) {
		case oscript::expr_node::type::CREATE:
			create.~create_expr_node();
			break;
		case oscript::expr_node::type::DESTROY:
			destroy.~destroy_expr_node();
			break;
		case oscript::expr_node::type::ID:
			id.~id_node();
			break;
		case oscript::expr_node::type::VALUE:
			value.~value_node();
			break;
	}
	tp = type::NONE;
}

void expr_node::read(const ast_node& node, const irep_error& err) {
	clear();
	switch (node.childs.Len()) {
		case 1:
			if (node.childs[0]->type == "id") {
				tp = type::ID;
				new (&id) id_node();
				id.read(*node.childs[0], err);
			} else {
				tp = type::VALUE;
				new (&value) value_node();
				value.read(*node.childs[0], err);
			}
			break;
		case 2:
			if (node.childs[0]->value == "~") {
				tp = type::DESTROY;
				new (&destroy) destroy_expr_node();
				destroy.read(node, err);
				break;
			} else {
				tp = type::CREATE;
				new (&create) create_expr_node();
				create.read(node, err);
				break;
			}

		default:
			err.ensure(0, "non supporting expression");
	};
}

expr_node::expr_node() {}

expr_node::expr_node(const expr_node& in) {
	this->operator=(in);
}

void expr_node::operator=(const expr_node& in) {
	clear();
	switch (in.tp) {
		case oscript::expr_node::type::CREATE:
			new (&create) create_expr_node(in.create);
			break;
		case oscript::expr_node::type::DESTROY:
			new (&destroy) destroy_expr_node(in.destroy);
			break;
		case oscript::expr_node::type::ID:
			new (&id) id_node(in.id);
			break;
		case oscript::expr_node::type::VALUE:
			new (&value) value_node(in.value);
	}
	tp = in.tp;
}

expr_node::~expr_node() {
	clear();
}

void while_loop_node::clear() {
	delete scope;
	scope = NULL;
}

void while_loop_node::read(const ast_node& node, const irep_error& err) {
	expr.read(*node.childs[2], err);
	scope = new scope_node();
	scope->read(*node.childs[4], err);
}

while_loop_node::~while_loop_node() {
	clear();
}

void ifelse_node::clear() {
	if (else_scope) {
		delete else_scope;
		else_scope = NULL;
	}
	delete scope;
	scope = NULL;
}

void ifelse_node::read(const ast_node& node, const irep_error& err) {
	scope = new scope_node();
	scope->read(*node.childs[4], err);

	if (node.childs[5]->childs.Len()) {
		else_scope = new scope_node();
		else_scope->read(*node.childs[5]->childs[1], err);
	}
	expr.read(*node.childs[2], err);
}

ifelse_node::~ifelse_node() {
	clear();
}

void code_block::clear() {
	switch (tp) {
		case type::IF:
			ifelse.~ifelse_node();
			break;
		case type::WHILE:
			whileloop.~while_loop_node();
			break;
		case type::SCOPE:
			delete scope;
			break;
		case type::EXPR:
			expr.~expr_node();
			break;
	}
}

code_block::~code_block() {
	clear();
}

void code_block::read(const ast_node& node, const irep_error& err) {
	clear();

	switch (node.childs.Len()) {
		case 6:
			tp = type::IF;
			new (&ifelse) ifelse_node();
			ifelse.read(node, err);
			break;
		case 5:
			tp = type::WHILE;
			new (&whileloop) while_loop_node();
			whileloop.read(node, err);
			break;
		case 2:
			tp = type::EXPR;
			new (&expr) expr_node();
			expr.read(*node.childs[0], err);
			break;
		case 1:
			tp = type::SCOPE;
			scope = new scope_node();
			scope->read(*node.childs[0], err);
			break;

		default:
			err.ensure(0, "unsupported code block");
	}
}

void code_block::operator=(const code_block& in) {
	clear();
	switch (in.tp) {
		case oscript::code_block::type::EXPR:
			new (&expr) expr_node(in.expr);
			break;
		case oscript::code_block::type::IF:
			new (&ifelse) ifelse_node(in.ifelse);
			break;
		case oscript::code_block::type::WHILE:
			new (&whileloop) while_loop_node(in.whileloop);
			break;
		case oscript::code_block::type::SCOPE:
			scope = new scope_node(*in.scope);
	}
}

void scope_node::read_code(const ast_node& node, const irep_error& err) {
	switch (node.childs.Len()) {
		case 2:
			read_code(*node.childs[0], err);
			code.PushBack(code_block());
			code.Last()->data.read(*node.childs[1], err);
			break;
		case 1:
			code.PushBack(code_block());
			code.Last()->data.read(*node.childs[0], err);
		default:
			break;
	}
}

void scope_node::read(const ast_node& node, const irep_error& err) {
	read_code(*node.childs[1], err);
}

void function_node::read_args(const ast_node& node, const irep_error& err) {
	switch (node.childs.Len()) {
		case 4:
			read_args(*node.childs[0], err);
			args.PushBack({node.childs[2]->value, node.childs[3]->value});
			break;
		case 2:
			args.PushBack({node.childs[0]->value, node.childs[1]->value});
		default:
			break;
	}
}

void function_node::read(const ast_node& node, const irep_error& err) {
	err.ensure(node.type == "function_def", "function expected");
	err.ensure(node.childs.Len() == 6, "invalid function signature");
	err.ensure(node.childs[0]->type == "id", "invalid ret type");
	err.ensure(node.childs[1]->type == "main_terminal", "invalid name");
	err.ensure(node.childs[3]->type == "func_args", "invalid argumments");
	err.ensure(node.childs[5]->type == "scope", "invalid function body");

	ret.id = node.childs[0]->value;
	name.id = node.childs[1]->value;

	read_args(*node.childs[3], err);
	body.read(*node.childs[5], err);
}

void treecode::read(const ast_node& node, const irep_error& err) {

	assert(node.childs.Len());

	entry.body.code.Clear();
	entry.args.Clear();

	err.ensure(node.childs.Len(), "No Entry");
	entry.read(*node.childs.First()->data, err);
}