

#include "oscript.h"


int main() {

	oscript_init();

	virtual_machine vm;
	
	string save_path = "A:/data/tmp/nd.nd";

	string string_type = "str";
	string int_type = "int";
	string method_type = "method";
	string list_type = "list";


	code func;
	func.Reserve(15);
	func[0]  = { OP_CREATE,  0, 0, 0, 0, 0, 0, 0, (alni)(&int_type)                                            }; // my_int = 0;
	func[1]  = { OP_CREATE,  1, 0, 0, 0, 0, 0, 0, (alni)(&list_type)                                           }; // my_list = [];
	func[2]  = { OP_CREATE,  2, 0, 0, 0, 0, 0, 0, (alni)(&string_type)                                         }; // save_path = "A:/data/tmp/nd.nd";
	func[3]  = { OP_SET,     2, 2, 0, 0, 0, 0, 0, (alni)(&save_path)                                           };
	func[4]  = { OP_CREATE,  3, 0, 0, 0, 0, 0, 0, (alni)(&method_type)                                         }; // def method() {};
	func[5]  = { OP_CALL,    3, 0, 0, 0, 0, 0, 0, (alni)(MethodObjectType.methods[METHOD_OBJECT_COPILE].adress)}; // method.compile();
	func[6]  = { OP_CALL,    1, 0, 0, 1, 0, 0, 0, (alni)(ListObjectType.methods[LISTOBJECT_PUSH_BACK].adress)  }; // my_list.push_back(method);
	func[7]  = { OP_CALL,    1, 0, 3, 1, 0, 0, 0, (alni)(ListObjectType.methods[LISTOBJECT_PUSH_BACK].adress)  }; // my_list.push_back(my_int);
	func[8]  = { OP_CALL,    1, 0, 0, 1, 0, 0, 0, (alni)(ListObjectType.methods[LISTOBJECT_GET_LENGTH].adress) }; // my_int = my_list.len();
	func[9]  = { OP_SAVE,    1, 2, 0, 0, 0, 0, 0, 0                                                            }; // save(my_list, save_path);
	func[10] = { OP_LOAD,    4, 2, 0, 0, 0, 0, 0, 0                                                            }; // loaded_list = load(save_path);
	func[11] = { OP_DESTROY, 0, 0, 0, 0, 0, 0, 0, 0                                                            }; //~my_int;
	func[12] = { OP_DESTROY, 1, 0, 0, 0, 0, 0, 0, 0                                                            }; // ~my_list;
	func[13] = { OP_DESTROY, 4, 0, 0, 0, 0, 0, 0, 0                                                            }; // ~method; 
	func[14] = { OP_DESTROY, 3, 0, 0, 0, 0, 0, 0, 0                                                            }; // ~loaded_list; // childs are unfreed

	vm.run_code(&func, object_optable);
}