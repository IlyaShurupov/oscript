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
		void read(const ast_node& node, const irep_error& err);
	};

	struct value_node {
		string val;
		void read(const ast_node& node, const irep_error& err);
	};

	struct creation_expr_node {
		bool raw;
		string type;
		string name;
		value_node val;
		void read(const ast_node& node, const irep_error& err);
	};

	struct ariphmetic_expr_node {
		enum class type { ASSIGN, ADD, SUB, MUL, DIV, ENCAPS, NONE } tp = type::NONE;
		struct expr_node* left = NULL;
		expr_node* right = NULL;

		void read(const ast_node& node, const irep_error& err);
		~ariphmetic_expr_node();
	};

	struct boolean_ariphmetic_expr_node {
		enum class type { EQUAL, GRATER, NOT, AND, OR, NONE } tp = type::NONE;
		expr_node* left = NULL;
		expr_node* right = NULL;

		void read(const ast_node& node, const irep_error& err);
		~boolean_ariphmetic_expr_node();
	};

	struct call_expr_node {
		expr_node* callable;
		Array<expr_node*> args;

		call_expr_node();
		void read_args(const ast_node& node, const irep_error& err);
		void read(const ast_node& node, const irep_error& err);
		~call_expr_node();
	};

	struct get_expr_node {
		expr_node* expr = NULL;
		id_node childname;

		get_expr_node();
		void read(const ast_node& node, const irep_error& err);
		~get_expr_node();
	};

	struct control_flow_expr_node {
		enum class type { RETURN, RETURN_NONE, BREAK, NONE } tp = type::NONE;
		expr_node* expr = NULL;

		void read(const ast_node& node, const irep_error& err);
		~control_flow_expr_node();
	};

	struct expr_node {
		enum class type { CREATION, ARIPHM, BOOL_ARIPHM, CALL, GET, CFLOW, NONE } tp = type::NONE;
		union {
			creation_expr_node create;
			ariphmetic_expr_node ariphm;
			boolean_ariphmetic_expr_node boolean_ariphm;
			call_expr_node call;
			get_expr_node get;
			control_flow_expr_node cflow;
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

		void evaluate(fbody*);
	};
};