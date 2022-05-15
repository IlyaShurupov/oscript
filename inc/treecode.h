#pragma once

#include "process.h"

#include "strings.h"
#include "list.h"

#include "parser.h"

namespace oscript {

	struct irep_error {
		virtual void ensure(bool expr, const string& def) const;
	};

	struct id_node {
		string id;
		void read(const ast_node& node, const irep_error& err) {
			err.ensure(node.type == "id", "expected identifier");
			id = node.value;
		}
	};

	struct value_node {
		string val;
		void read(const ast_node& node, const irep_error& err) {
			val = node.value;
		}
	};

	struct create_expr_node {
		id_node type;
		id_node name;
		void read(const ast_node& node, const irep_error& err);
	};

	struct destroy_expr_node {
		struct expr_node* del_expr = NULL;
		void read(const ast_node& node, const irep_error& err);
		~destroy_expr_node();
	};

	struct expr_node {
		enum class type { CREATE, DESTROY, ID, VALUE, NONE } tp = type::NONE;
		union {
			create_expr_node create;
			destroy_expr_node destroy;
			id_node id;
			value_node value;
		};
		expr_node();
		expr_node(const expr_node& in);
		void operator=(const expr_node& in);
		void clear();
		void read(const ast_node& node, const irep_error& err);
		~expr_node();
	};

	struct while_loop_node {
		expr_node expr;
		struct scope_node* scope = NULL;
		void read(const ast_node& node, const irep_error& err);
		void clear();
		~while_loop_node();
	};

	struct ifelse_node {
		expr_node expr;
		scope_node* scope = NULL;
		scope_node* else_scope = NULL;
		void read(const ast_node& node, const irep_error& err);
		void clear();
		~ifelse_node();
	};

	struct code_block {
		code_block() {}
		union {
			expr_node expr;
			ifelse_node ifelse;
			while_loop_node whileloop;
			scope_node* scope = NULL;
		};
		enum class type { EXPR, IF, WHILE, SCOPE, NONE } tp = type::NONE;
		void read(const ast_node& node, const irep_error& err);
		void operator=(const code_block& in);
		void clear();
		~code_block();
	};

	struct scope_node {
		list<code_block> code;
		void read_code(const ast_node& node, const irep_error& err);
		void read(const ast_node& node, const irep_error& err);
	};

	struct argument_node {
		string type;
		string id;
	};

	struct function_node {
		id_node name;
		id_node ret;
		list<argument_node> args;
		scope_node body;
		void read_args(const ast_node& node, const irep_error& err);
		void read(const ast_node& node, const irep_error& err);
	};

	struct treecode {
		function_node entry;
		void read(const ast_node& node, const irep_error& err);

		void evaluate(fbody*) {
			return;
		}
	};
};