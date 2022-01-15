

#include "vm.h"

#include "optable.h"
#include "primitives/primitives.h"


int main() {

	objects_api* oh = objects_init();
	primitives_define_types(oh);

	virtual_machine vm;
	
	string type = "int";
	string list_type = "list";

	code func;
	func.Reserve(6);
	func[0] = { OP_CREATE, 0, 0, 0, 0, 0, 0, 0, (alni)(&type) };
	func[1] = { OP_CREATE, 1, 0, 0, 0, 0, 0, 0, (alni)(&list_type) };
	func[2] = { OP_CALL, 1, LISTOBJECT_PUSH_BACK, 0, 0, 1, 0, 0, (alni)(&list_type) };
	func[3] = { OP_CALL, 1, LISTOBJECT_GET_LENGTH, 0, 0, 1, 0, 0, (alni)(&list_type) };
	func[4] = { OP_DESTROY, 1, 0, 0, 0, 0, 0, 0, 0 };
	func[5] = { OP_DESTROY, 0, 0, 0, 0, 0, 0, 0, 0 };

	vm.run_code(&func, object_optable);
}