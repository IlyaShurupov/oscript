#pragma once

#include "types.h"

enum token_type {
	DIG,
	ID,
};

struct token {
	token_type toktype;
	string val;
};


struct DSA {

};

struct oscript_scaner {

	string* stream = NULL;
	alni pos = 0;



	void set_stream(string* input_stream, alni pos = 0) {
		stream = input_stream;
		this->pos = pos;
	}

	token get_token() {
		if (stream->str[pos]) {
		}
	}
};

struct oscript_parser {

	void parse() {
	}

};

struct oscript_compiler {

	void parse() {
	}

};