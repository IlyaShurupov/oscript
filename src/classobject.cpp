#pragma once


#include "classobject.h"


void ClassObject::constructor(ClassObject* self) {}
void ClassObject::copy(ClassObject* self, const ClassObject* in) {}
void ClassObject::destructor(ClassObject* self) {}
static alni save_size(ClassObject* self) { return 0; }
static void save(ClassObject* self, File& file_self) {}
static void load(File& file_self, ClassObject* self) {}


static void class_object_call_method_object(ClassObject* self, object_caller* caller) {
}

type_method ClassObjectTypeMethods[] = {
	{"call", (type_method_adress*)class_object_call_method_object},
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
