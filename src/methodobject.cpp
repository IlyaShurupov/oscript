#pragma once

/*

#include "methodobject.h"

struct MemScriptSection {
	List<script_data*> scripts;

	script_data* get_script() {
		//script_data* out = new scri
		return NULL;
	}

	void release_script(script_data*) {
	}
};

MemScriptSection ScriptSection;

void MethodObject::constructor(MethodObject* self) {
	self->script = NULL;
}

void MethodObject::copy(MethodObject* self, const MethodObject* in) {
	self->script = in->script;
	self->self = in->self;
}

void MethodObject::destructor(MethodObject* self) {
	ScriptSection.release_script(self->script);
}

static alni save_size(MethodObject* self) {
	alni len = self->items.Len();
	return (len + 1) * sizeof(alni);
}

static void save(MethodObject* self, File& file_self) {

	alni len = self->items.Len();
	file_self.write<alni>(&len);

	for (auto item : self->items) {
		alni ndo_object_adress = NDO->save(file_self, item.Data());
		file_self.write<alni>(&ndo_object_adress);
	}
}

static void load(File& file_self, MethodObject* self) {

	new (&self->items) List<Object*>();

	alni len;
	file_self.read<alni>(&len);

	for (alni i = 0; i < len; i++) {
		alni ndo_object_adress;
		file_self.read<alni>(&ndo_object_adress);
		self->items.PushBack(NDO->load(file_self, ndo_object_adress));
	}
}


void method_object_call_method_object(Object* self, object_caller* caller) {
	//NDO_CAST(IntObject, caller->get(0))->val = NDO_CAST(ListObject, self)->items.Len();
}

void method_object_compile_script(Object* self, object_caller* caller) {
	//NDO_CAST(ListObject, self)->items.PushBack(caller->get(0));
}

type_method* MethodObjectTypeMethods[] = {
	method_object_call_method_object,
	method_object_compile_script,
};


struct ObjectType ListObjectType = {
	.base = NULL,
	.constructor = (object_constructor)MethodObject::constructor,
	.destructor = (object_destructor)MethodObject::destructor,
	.copy = (object_copy)MethodObject::copy,
	.size = sizeof(MethodObject),
	.name = "method",
	.convesions = NULL,
	.save_size = (object_save_size)save_size,
	.save = (object_save)save,
	.load = (object_load)load,
	.methods = MethodObjectTypeMethods,
};

*/