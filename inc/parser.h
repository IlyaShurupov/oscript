#pragma once

#include "list.h"
#include "string.h"

namespace osc {

	struct ast_node {
		tp::List<ast_node*> childs;
		tp::string type;
		tp::string value;
		bool terminal = false;

		~ast_node() {
			for (auto child : childs) {
				delete child.data();
			}
		}

		tp::alni plen() const { return childs.length(); }
		const ast_node& operator[](tp::alni idx) const { return *childs[idx]; }
	};

	struct parser {

		parser(const tp::string& grammar);
		ast_node* parse(const tp::string& stream);
		~parser();

		private: void* grammar_compiler = NULL;
		private: void* lalr_parser = NULL;
	};

};