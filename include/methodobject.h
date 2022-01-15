#pragma once

/*

#include "object/object.h"
#include "primitives/stringobject.h"

#include "vm.h"

enum MethodObjectTypeMethods {
	METHOD_OBJECT_CALL_METHOD_OBJECT,
	METHOD_OBJECT_COPILE,
	METHOD_OBJECT_GET_SCRIPT,
	METHOD_OBJECT_SET_SELF,
};

struct script_data {
	StringObject* script;
	code* bytecode;
};

struct MethodObject : Object {
	
	Object* self;
	script_data* script;

	static void constructor(MethodObject* self);
	static void copy(MethodObject* self, const MethodObject* in);
	static void destructor(MethodObject* self);

};


extern ObjectType MethodObjectType;

*/