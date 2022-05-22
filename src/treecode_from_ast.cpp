
#include "treecode.h"

namespace osc {

	void id_node::read(const ast_node& node, const irep_error& err) {
		err.ensure(node.type == "id", "expected identifier");
		id = node.value;
	}

	void value_node::read(const ast_node& node, const irep_error& err) {
		val = node.value;
	}

	void creation_expr_node::read(const ast_node& node, const irep_error& err) {
		raw = node.plen() == 1;
		if (!raw) {
			type.read(node[0], err);
			name.read(node[1], err);
		} else {
			val.read(node[0], err);
		}
	}

	void ariphmetic_expr_node::read(const ast_node& node, const irep_error& err) {
		if (node.plen() == 2) {
			tp = type::ENCAPS;
			left = new expr_node();
			left->read(node[1], err);
		} else {
			left = new expr_node();
			right = new expr_node();
			left->read(node[0], err);
			right->read(node[2], err);

			tp::string optype = node[1].value;
			if (optype == "+") {
				tp = type::ADD;
			} else if (optype == "-") {
				tp = type::SUB;
			} else if (optype == "*") {
				tp = type::MUL;
			} else if (optype == "/") {
				tp = type::DIV;
			} else if (optype == "=") {
				tp = type::ASSIGN;
			} else {
				err.ensure(0, "unsupported ariphmetic operator");
			}
		}

	}

	ariphmetic_expr_node::~ariphmetic_expr_node() {
		if (tp != type::NONE) {
			if (right) delete right;
		}
		if (left) delete left;
	}

	void boolean_ariphmetic_expr_node::read(const ast_node& node, const irep_error& err) {
		if (node.plen() == 4) {
			tp = type::NOT;
			left = new expr_node();
			left->read(node[2], err);
		} else {
			left = new expr_node();
			right = new expr_node();
			left->read(node[0], err);
			right->read(node[2], err);

			tp::string optype = node[1].value;
			if (optype == "&&") {
				tp = type::AND;
			} else if (optype == "||") {
				tp = type::OR;
			} else if (optype == ">") {
				tp = type::GRATER;
			} else if (optype == "==") {
				tp = type::EQUAL;
			} else {
				err.ensure(0, "unsupported ariphmetic operator");
			}
		}
	}

	boolean_ariphmetic_expr_node::~boolean_ariphmetic_expr_node() {
		if (tp != type::NONE) {
			if (right) delete right;
		}
		if (left) delete left;
	}

	call_expr_node::call_expr_node() {
		callable = new expr_node();
	}

	void call_expr_node::read_args(const ast_node& node, const irep_error& err) {
		switch (node.plen()) {
			case 0:
			{
				return;
			}
			case 1:
			{
				expr_node* arg = new expr_node();
				arg->read(node[0], err);
				args.pushBack(arg);
				return;
			}
			case 3:
			{
				read_args(node[0], err);
				expr_node* arg = new expr_node();
				arg->read(node[2], err);
				args.pushBack(arg);
				return;
			}
			default:
			{
				assert(0 && "unhandled control flow");
			}
		}
	}

	void call_expr_node::read(const ast_node& node, const irep_error& err) {
		callable->read(node[0], err);
		read_args(node[2], err);
	}

	call_expr_node::~call_expr_node() {
		if (callable) delete callable;
		for (auto i : args) {
			delete i.data();
		}
	}

	get_expr_node::get_expr_node() {
		expr = new expr_node();
	}

	void get_expr_node::read(const ast_node& node, const irep_error& err) {
		expr->read(node[0], err);
		childname.read(node[2], err);
	}

	get_expr_node::~get_expr_node() {
		if (expr) delete expr;
	}

	void control_flow_expr_node::read(const ast_node& node, const irep_error& err) {
		if (node.plen() == 1) {
			if (node[0].value == "break") {
				tp = type::BREAK;
			} else {
				tp = type::RETURN_NONE;
			}
		} else {
			tp = type::RETURN;
			expr = new expr_node();
			expr->read(node[0], err);
			err.ensure(expr->has_return, "invalid expresion");
		}
	}

	control_flow_expr_node::~control_flow_expr_node() {
		if (expr) delete expr;
	}

	void expr_node::clear() {
		switch (tp) {
			case expr_node::type::CREATION:
				create.~creation_expr_node();
				break;
			case expr_node::type::ARIPHM:
				ariphm.~ariphmetic_expr_node();
				break;
			case expr_node::type::BOOL_ARIPHM:
				boolean_ariphm.~boolean_ariphmetic_expr_node();
				break;
			case expr_node::type::CALL:
				call.~call_expr_node();
				break;
			case expr_node::type::GET:
				get.~get_expr_node();
				break;
			case expr_node::type::CFLOW:
				cflow.~control_flow_expr_node();
				break;
		}
		tp = type::NONE;
	}

	void expr_node::read(const ast_node& node, const irep_error& err) {
		clear();
		const ast_node& expr_type = node[0];
		if (expr_type.type == "creation_expr") {
			tp = type::CREATION;
			new (&create) creation_expr_node();
			create.read(expr_type, err);
		} else if (expr_type.type == "ariphmetic_expr") {
			tp = type::ARIPHM;
			new (&ariphm) ariphmetic_expr_node();
			ariphm.read(expr_type, err);
		} else if (expr_type.type == "boolen_ariphm_expr") {
			tp = type::BOOL_ARIPHM;
			new (&boolean_ariphm) boolean_ariphmetic_expr_node();
			boolean_ariphm.read(expr_type, err);
		} else if (expr_type.type == "get_expr") {
			tp = type::GET;
			new (&get) get_expr_node();
			get.read(expr_type, err);
		} else if (expr_type.type == "call_expr") {
			tp = type::CALL;
			new (&call) call_expr_node();
			call.read(expr_type, err);
		} else if (expr_type.type == "control_flow_expr") {
			tp = type::CFLOW;
			new (&cflow) control_flow_expr_node();
			cflow.read(expr_type, err);
		} else {
			err.ensure(0, "not supporting expression");
		}
		has_return = tp == expr_node::type::BOOL_ARIPHM || tp == expr_node::type::ARIPHM || tp == expr_node::type::CALL;
	}

	expr_node::expr_node() {}

	expr_node::expr_node(const expr_node& in) {
		this->operator=(in);
	}

	void expr_node::operator=(const expr_node& in) {
		clear();
		switch (tp) {
			case expr_node::type::CREATION:
				new (&create) creation_expr_node(in.create);
				break;
			case expr_node::type::ARIPHM:
				new (&ariphm) ariphmetic_expr_node(in.ariphm);
				break;
			case expr_node::type::BOOL_ARIPHM:
				new (&boolean_ariphm) boolean_ariphmetic_expr_node(in.boolean_ariphm);
				break;
			case expr_node::type::CALL:
				new (&call) call_expr_node(in.call);
				break;
			case expr_node::type::GET:
				new (&get) get_expr_node(in.get);
				break;
			case expr_node::type::CFLOW:
				new (&cflow) control_flow_expr_node(in.cflow);
				break;
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
		expr.read(node[2], err);
		err.ensure(expr.has_return, "invalid expresion");

		scope = new scope_node();
		scope->read(node[4], err);
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
		scope->read(node[4], err);

		if (node[5].plen()) {
			else_scope = new scope_node();
			else_scope->read(node[5][1], err);
		}

		expr.read(node[2], err);
		err.ensure(expr.has_return, "invalid expresion");
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

		switch (node.plen()) {
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
				expr.read(node[0], err);
				break;
			case 1:
				tp = type::SCOPE;
				scope = new scope_node();
				scope->read(node[0], err);
				break;

			default:
				err.ensure(0, "unsupported code block");
		}
	}

	void code_block::operator=(const code_block& in) {
		clear();
		switch (in.tp) {
			case code_block::type::EXPR:
				new (&expr) expr_node(in.expr);
				break;
			case code_block::type::IF:
				new (&ifelse) ifelse_node(in.ifelse);
				break;
			case code_block::type::WHILE:
				new (&whileloop) while_loop_node(in.whileloop);
				break;
			case code_block::type::SCOPE:
				scope = new scope_node(*in.scope);
		}
	}

	void scope_node::read_code(const ast_node& node, const irep_error& err) {
		switch (node.plen()) {
			case 2:
				read_code(node[0], err);
				code.pushBack(code_block());
				code.last()->data.read(node[1], err);
				break;
			case 1:
				code.pushBack(code_block());
				code.last()->data.read(node[0], err);
			default:
				break;
		}
	}

	void scope_node::read(const ast_node& node, const irep_error& err) {
		read_code(node[1], err);
	}

	void function_node::read_args(const ast_node& node, const irep_error& err) {
		argument_node new_arg;
		switch (node.plen()) {
			case 4:
				read_args(node[0], err);
				new_arg.type = node[2].value;
				new_arg.name.read(node[3], err);
				args.pushBack(new_arg);
				break;
			case 2:
				new_arg.type = node[0].value;
				new_arg.name.read(node[1], err);
				args.pushBack(new_arg);
			default:
				break;
		}
	}

	void function_node::read(const ast_node& node, const irep_error& err) {
		err.ensure(node.type == "function_def", "function expected");
		err.ensure(node.plen() == 6, "invalid function signature");
		err.ensure(node[0].type == "id", "invalid ret type");
		err.ensure(node[1].type == "main_terminal", "invalid name");
		err.ensure(node[3].type == "func_args", "invalid argumments");
		err.ensure(node[5].type == "scope", "invalid function body");

		ret.id = node[0].value;
		name.id = node[1].value;

		read_args(node[3], err);
		body.read(node[5], err);
	}

	void treecode::read(const ast_node& node, const irep_error& err) {

		assert(node.plen());

		entry.body.code.free();
		entry.args.free();

		err.ensure(node.plen(), "No Entry");
		entry.read(node[0], err);
	}

};