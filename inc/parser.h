#pragma once

#include "list.h"
#include "string.h"

namespace oscript {

	struct ast_node {
		list<ast_node*> childs;
		string type;
		string value;
		bool terminal = false;

		~ast_node() {
			for (auto child : childs) {
				delete child.Data();
			}
		}
	};

	struct parser {

		parser(const string& grammar);
		ast_node* parse(const string& stream);
		~parser();

		private: void* grammar_compiler = NULL;
		private: void* lalr_parser = NULL;
	};

};