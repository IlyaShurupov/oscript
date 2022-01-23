#pragma once

#include "types.h"

enum scanner_tok_types {
	TOK_OP,
	TOK_CP,
	TOK_STAR,
	TOK_OR,
	TOK_CHAR,
	TOK_END,
};

struct token {
	token(alni toktype, string val) : toktype(toktype), val(val) {}

	alni toktype;
	string val;
};

struct production {

	production(ASTNode* nonterminal) {
		this->nonterminal = nonterminal;
	}

	production(token* terminal) {
		this->terminal = terminal;
	}

	ASTNode* nonterminal = NULL;
	token* terminal = NULL;

	~production() {
		delete nonterminal;
		delete terminal;
	}
};

struct ASTNode {
	Array<production> productions;
};

struct FAState {
	Array<FAState*> paths;
	bool (*can_transition)(alni toktype) = NULL;

	FAState(bool (*can_transition)(alni)) {
		this->can_transition = can_transition;
	}

	FAState* move_state(alni toktype) {
		for (alni i = 0; i < paths.Len(); i++) {
			if (paths[i]->can_transition(toktype)) {
				return paths[i];
			}
		}

		throw 0;
	}

};

struct re_parser {
	
	string* stream = NULL;
	alni len = 0;
	alni pos = 0;

	token la_tok(bool offset = true) {

		if (len == pos) {
			return token(TOK_END, "");
		}

		switch (stream->str[pos]) {
			case '*':
				return token(TOK_STAR, "*");
			case '(':
				return token(TOK_OP, "(");
			case ')':
				return token(TOK_CP, ")");
			case '|':
				return token(TOK_OR, "|");
			default:
				return token(TOK_CHAR, stream->str[pos]);
		}

		pos += offset;
	}

	void check_er(bool ifstm, string err, ASTNode* node) {
		// does not clears allocated data
		if (ifstm) {
			delete node;
			throw err;
		}
	}

	ASTNode* parse(string* stream, alni pos = 0) {

		this->stream = stream;
		this->len = stream->len();
		this->pos = pos;
		
		ASTNode* root = new ASTNode();
		if (parse_exprassion(root)) {
			return root;
		}
		return NULL;
	}
	
	bool parse_exprassion(ASTNode* parent) {

		token tok(0, "");
		do {
			ASTNode* astnode = new ASTNode();
			tok = la_tok(0);

			switch (tok.toktype) {
				case TOK_OP: {

					if (!parse_opcp(astnode)) {
						return false;
					}
					break;
				}

				case TOK_CHAR: {
					ASTNode* char_node = new ASTNode();
					char_node->productions.PushBack(new token(la_tok()));
					astnode->productions.PushBack(astnode);
					break;
				}
			}

			parent->productions.PushBack(astnode);

		} while (tok.toktype != TOK_END);
	}

	bool parse_opcp(ASTNode* parent) {
		
		ASTNode* astnode = new ASTNode();

		token op_tok = la_tok();

		if (!parse_exprassion(astnode)) {
			delete astnode;
			return false;
		}

		token cp_tok = la_tok();

		if (cp_tok.toktype != TOK_CP) {
			delete astnode;
			return false;
		}

		ASTNode* opcp_node = new ASTNode();
		opcp_node->productions.PushBack(new token(op_tok));
		opcp_node->productions.PushBack(astnode);
		opcp_node->productions.PushBack(new token(cp_tok));

		token tok = la_tok();

		switch (tok.toktype) {
			case TOK_STAR: {
				ASTNode* star_node = new ASTNode();

				star_node->productions.PushBack(opcp_node);
				star_node->productions.PushBack(new token(tok));

				parent->productions.PushBack(parent);
				break;
			}
			case TOK_OR: {
				ASTNode* or_node = new ASTNode();

				or_node->productions.PushBack(opcp_node);
				or_node->productions.PushBack(new token(tok));

				ASTNode* right_node = new ASTNode();

				if (!parse_exprassion(right_node)) {
					delete right_node;
					return false;
				}

				or_node->productions.PushBack(right_node);

				parent->productions.PushBack(or_node);
				break;
			}
			case TOK_CHAR: 
			case TOK_OP: 
			case TOK_END: {
				parent->productions.PushBack(opcp_node);
				break;
			}
		}
		
		return true;
	}
};

struct token_description {
	string re;
	alni toktype;
};

typedef Array<token_description> scanner_grammar;

struct re_compiler {

	re_parser parser;

	FAState* compile_to_nfa(token_description* tok_desc) {
		ASTNode* root = parser.parse(&tok_desc->re);
		return NULL;
	}

	FAState* convert_NFAs_to_DFA(List<FAState*> nfas) {
		return NULL;
	}

	FAState* compile(scanner_grammar grammar) {

		List<FAState*> nfas;

		for (auto tok_des : grammar) {
			nfas.PushBack(compile_to_nfa(&tok_des.data()));
		}

		return convert_NFAs_to_DFA(nfas);
	}
};

struct oscript_scaner {

};
