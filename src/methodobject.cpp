#pragma once

#include "methodobject.h"

#include "compiler.h"

void MethodObject::constructor(MethodObject* self) {
	self->script = ScriptSection.new_script();
	self->script->string = (StringObject*)NDO->create("str");
}

void MethodObject::copy(MethodObject* self, const MethodObject* in) {
	ScriptSection.change_script(self->script, in->script);
	self->script = in->script;
}

void MethodObject::destructor(MethodObject* self) {
	ScriptSection.abandon_script(self->script);
}

static alni save_size(MethodObject* self) {
	// script_table_file_address & script string object address
	return sizeof(alni) * 2; 
}

static void save(MethodObject* self, File& file_self) {

	// script_table_file_address 
	alni script_table_file_address = ScriptSection.get_script_file_adress(self->script);
	file_self.write<alni>(&script_table_file_address);

	// script string object address
	alni script_string_object = NDO->save(file_self, self->script->string);
	file_self.write<alni>(&script_string_object);
}

static void load(File& file_self, MethodObject* self) {

	// script_table_file_address 
	alni script_table_file_address;
	file_self.read<alni>(&script_table_file_address);
	self->script = ScriptSection.get_scritp_from_file_adress(script_table_file_address);

	// script string object address
	alni script_string_object;
	file_self.read<alni>(&script_string_object);
	self->script->string = (StringObject*)NDO->load(file_self, script_string_object);
}


void method_object_compile_script(MethodObject* self, object_caller* caller) {
	oscript::compiler cmp;
	cmp.compile(self->script->string->val, &self->script->code);
}

void method_object_get_script(MethodObject* self, object_caller* caller) {
	caller->ret(self->script->string);
}

type_method MethodObjectTypeMethods[] = {
	"compile", (type_method_adress*)method_object_compile_script,
	"string", (type_method_adress*) method_object_get_script,
};


struct ObjectType MethodObjectType = {
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
