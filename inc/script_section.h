#pragma once

#include "object/object.h"
#include "primitives/stringobject.h"

#include "methodobject.h"

struct MemScriptSection {
	list<script_data*> scripts;

	MemScriptSection() {}

	script_data* new_script();
	void reference_script(script_data* script);
	void change_script(script_data* current_script, script_data* new_script);
	void abandon_script(script_data* script);


	static void save_script_table_to_file(MemScriptSection* self, File& file);
	alni get_script_file_adress(script_data* in);
	static void load_script_table_from_file(MemScriptSection* self, File& file);
	script_data* get_scritp_from_file_adress(alni file_adress); \

	~MemScriptSection();
};

extern MemScriptSection ScriptSection;

extern save_load_callbacks slcb_script_section;