#pragma once

#include "object/object.h"
#include "primitives/stringobject.h"

#include "vm.h"

struct script_data {
	StringObject* script = NULL;
	code bytecode;
};

enum MethodObjectTypeMethods {
	METHOD_OBJECT_COPILE,
	METHOD_OBJECT_GET_SCRIPT,
};

struct MethodObject : Object {
	
	script_data* script;

	static void constructor(MethodObject* self);
	static void copy(MethodObject* self, const MethodObject* in);
	static void destructor(MethodObject* self);
};


extern ObjectType MethodObjectType;