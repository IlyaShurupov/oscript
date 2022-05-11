
#include "treecode.h"

using namespace oscript;

void irep_error::ensure(bool expr, const string& def) const {
	if (!expr) {
		rterror((string("oscript parser error - ") + def).cstr());
	}
}

void expr_node::read(const ast_node& node, const irep_error& err) {
	return;
}

void code_block::read(const ast_node& node, const irep_error& err) {
	err.ensure(node.childs.Len() == 2, "not supporting cflow or nested scopes");
	expr.read(*node.childs[0], err);
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
