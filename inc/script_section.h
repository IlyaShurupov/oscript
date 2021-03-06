#pragma once

#include "object/object.h"
#include "primitives/stringobject.h"

#include "bytecode.h"

struct script_data {
	StringObject* string = NULL;
	fbody code;
};

struct MemScriptSection {
	list<script_data*> scripts;

	MemScriptSection() {}

	script_data* new_script();
	void delete_script(script_data* script);
	void reference_script(script_data* script);
	void change_script(script_data* current_script, script_data* new_script);
	void abandon_script(script_data* script);

	alni get_script_file_adress(script_data* in);
	script_data* get_scritp_from_file_adress(alni file_adress);

	static void save_script_table_to_file(MemScriptSection* self, File& file);
	static void load_script_table_from_file(MemScriptSection* self, File& file);

	~MemScriptSection();
};

extern MemScriptSection ScriptSection;
extern save_load_callbacks slcb_script_section;