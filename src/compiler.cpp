
#include "compiler.h"

#include "lalr/Parser.hpp"

#include "oscript.h"

void osc_init() {
}

string string_type = "str";
string int_type = "int";
string method_type = "method";
string list_type = "list";
string save_path = "A:/src/oscript/example/nd.nd";

void osc_compile(code* out, string* oscript) {

	out->Reserve(16);
	(*out)[0] = { OP_CREATE,  0, 0, 0, 0, 0, 0, 0, (alni)(&int_type) }; // my_int = 0;
	(*out)[1] = { OP_CREATE,  1, 0, 0, 0, 0, 0, 0, (alni)(&list_type) }; // my_list = [];
	(*out)[2] = { OP_CREATE,  2, 0, 0, 0, 0, 0, 0, (alni)(&string_type) }; // save_path = "A:/data/tmp/nd.nd";
	(*out)[3] = { OP_SET,     2, 2, 0, 0, 0, 0, 0, (alni)(&save_path) };
	(*out)[4] = { OP_CREATE,  3, 0, 0, 0, 0, 0, 0, (alni)(&method_type) }; // def method() {};
	(*out)[5] = { OP_CALL,    3, 0, 0, 0, 0, 0, 0, (alni)(MethodObjectType.methods[METHOD_OBJECT_COPILE].adress) }; // method.compile();
	(*out)[6] = { OP_CALL,    1, 0, 0, 1, 0, 0, 0, (alni)(ListObjectType.methods[LISTOBJECT_PUSH_BACK].adress) }; // my_list.push_back(method);
	(*out)[7] = { OP_CALL,    1, 0, 3, 1, 0, 0, 0, (alni)(ListObjectType.methods[LISTOBJECT_PUSH_BACK].adress) }; // my_list.push_back(my_int);
	(*out)[8] = { OP_CALL,    1, 0, 0, 1, 0, 0, 0, (alni)(ListObjectType.methods[LISTOBJECT_GET_LENGTH].adress) }; // my_int = my_list.len();
	(*out)[9] = { OP_SAVE,    1, 2, 0, 0, 0, 0, 0, 0 }; // save(my_list, save_path);
	(*out)[10] = { OP_LOAD,    4, 2, 0, 0, 0, 0, 0, 0 }; // loaded_list = load(save_path);
	(*out)[11] = { OP_DESTROY, 0, 0, 0, 0, 0, 0, 0, 0 }; //~my_int;
	(*out)[12] = { OP_DESTROY, 1, 0, 0, 0, 0, 0, 0, 0 }; // ~my_list;
	(*out)[13] = { OP_DESTROY, 2, 0, 0, 0, 0, 0, 0, 0 }; // ~my_list;
	(*out)[14] = { OP_DESTROY, 3, 0, 0, 0, 0, 0, 0, 0 }; // ~method; 
	(*out)[15] = { OP_DESTROY, 4, 0, 0, 0, 0, 0, 0, 0 };
}

void osc_finalize() {
}
