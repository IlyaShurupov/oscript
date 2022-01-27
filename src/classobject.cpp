#pragma once


#include "classobject.h"

#include "methodobject.h"

#include "optable.h"

void ClassObject::constructor(ClassObject* self) {}
void ClassObject::copy(ClassObject* self, const ClassObject* in) {}
void ClassObject::destructor(ClassObject* self) {}
static alni save_size(ClassObject* self) { return 0; }
static void save(ClassObject* self, File& file_self) {}
static void load(File& file_self, ClassObject* self) {}

struct call_stack_frame {

	call_stack_frame(alni  bp, alni  ip, code* code, Object* self, alni  ret, alni  args, alni  count) :
		caller_bp(bp),
		caller_ip(ip),
		caller_code(code),
		self(self),
		ret_address(ret),
		args_address(args),
		args_count(count)
	{}

	alni  caller_bp;
	alni  caller_ip;
	code* caller_code;

	Object* self;
	alni  ret_address;
	alni  args_address;
	alni  args_count;
};

static void class_object_call_method_object(ClassObject* self, type_method_caller* caller) {

	NDO_CASTV(MethodObject, caller->get(0), method);

	call_stack& cs = caller->proc->cstack;
	process& proc = *caller->proc;

	//Object* ret_adress = caller->get(0);
	//alni args_adress = caller->get(0);

	new (&cs.mem[cs.fp]) call_stack_frame(cs.bp, proc.ip, proc.fbody, self, 0, 0, 0);

	cs.bp = cs.fp;
	cs.fp = 0;

	proc.fbody = &method->script->bytecode;
	proc.ip = 0;
}

static void class_object_return_from_method_object(ClassObject* self, type_method_caller* caller) {
	//caller->proc
}

type_method ClassObjectTypeMethods[] = {
	{"call", (type_method_adress*)class_object_call_method_object},
	{"ret", (type_method_adress*)class_object_return_from_method_object},
};


struct ObjectType ClassObjectType = {
	.base = &DictObjectType,
	.constructor = (object_constructor)ClassObject::constructor,
	.destructor = (object_destructor)ClassObject::destructor,
	.copy = (object_copy)ClassObject::copy,
	.size = sizeof(ClassObject),
	.name = "class",
	.convesions = NULL,
	.save_size = (object_save_size)save_size,
	.save = (object_save)save,
	.load = (object_load)load,
	.methods = ClassObjectTypeMethods,
};