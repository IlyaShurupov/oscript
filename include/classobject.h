#pragma once


#include "object/object.h"
#include "primitives/dictobject.h"

enum ClassObjectTypeMethods {
	CLASS_OBJECT_CALL_METHOD_OBJECT,
};

struct ClassObject : DictObject {
	
	static void constructor(ClassObject* self);
	static void copy(ClassObject* self, const ClassObject* in);
	static void destructor(ClassObject* self);
};


extern ObjectType ClassObjectType;