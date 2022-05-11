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
	};

	struct create_expr_node {
		id_node type;
		id_node name;
	};

	struct destroy_expr_node {
		id_node name;
	};

	struct expr_node {
		union expr {
			create_expr_node create;
		};
		void read(const ast_node& node, const irep_error& err);
	};

	struct code_block {
		expr_node expr;
		void read(const ast_node& node, const irep_error& err);
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

		fbody* evaluate() {
			return NULL;
		}
	};
};