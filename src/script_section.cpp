
#include "script_section.h"

MemScriptSection ScriptSection;

struct script_data_head {
	alni refc = 0;
	alni store_adress = 0;
};

void set_script_head_store_adress(script_data* in, alni address) {
	((script_data_head*) in - 1)->store_adress = address;
}

alni get_script_head_store_adress(script_data* in) {
	return ((script_data_head*) in - 1)->store_adress;
}

script_data* MemScriptSection::new_script() {
	script_data_head* sdhead = (script_data_head*) malloc(sizeof(script_data_head) + sizeof(script_data));
	script_data* out = (script_data*) (sdhead + 1);

	if (!sdhead) {
		return NULL;
	}

	sdhead->refc = 1;
	sdhead->store_adress = -1;

	out->string = NULL;
	new (&out->code) fbody();

	scripts.PushBack(out);

	return out;
}

void MemScriptSection::delete_script(script_data* script) {
	script->~script_data();
	free((((script_data_head*) script) - 1));
}

void MemScriptSection::reference_script(script_data* script) {
	(((script_data_head*) script) - 1)->refc++;
}

void MemScriptSection::abandon_script(script_data* script) {
	if ((((script_data_head*) script) - 1)->refc > 1) {
		(((script_data_head*) script) - 1)->refc--;
	} else {
		delete_script(script);
	}
}

void MemScriptSection::change_script(script_data* current_script, script_data* new_script) {
	abandon_script(current_script);
	reference_script(new_script);
}

void MemScriptSection::save_script_table_to_file(MemScriptSection* self, File& file) {
	file.write_bytes("// script table //", 18);

	alni scripts_count = self->scripts.Len();
	file.write<alni>(&scripts_count);

	for (auto iter : self->scripts) {

		set_script_head_store_adress(iter.Data(), file.adress);

		// save code
		iter->code.bc.buff.save(file);
		iter->code.cd.mem.save(file);
	}

	file.write_bytes("// script table //", 18);
}

alni MemScriptSection::get_script_file_adress(script_data* in) {
	return  get_script_head_store_adress(in);
}

void MemScriptSection::load_script_table_from_file(MemScriptSection* self, File& file) {

	for (auto iter : self->scripts) {
		set_script_head_store_adress(iter.Data(), -1);
	}

	file.adress += 18;

	alni scripts_count;
	file.read<alni>(&scripts_count);

	for (alni i = 0; i < scripts_count; i++) {

		script_data* new_script = self->new_script();
		self->abandon_script(new_script);

		set_script_head_store_adress(new_script, file.adress);

		// read code
		new_script->code.bc.buff.load(file);
		new_script->code.cd.mem.load(file);
	}

	file.adress += 18;
}

script_data* MemScriptSection::get_scritp_from_file_adress(alni file_adress) {
	for (auto iter : scripts) {
		if (get_script_head_store_adress(iter.Data()) == file_adress) {
			return iter.Data();
		}
	}
	return NULL;
}

MemScriptSection::~MemScriptSection() {
	for (auto iter : scripts) {
		delete_script(iter.Data());
	}
}

save_load_callbacks slcb_script_section = {
	.self = &ScriptSection,
	.pre_save = (pre_save_callback*) MemScriptSection::save_script_table_to_file,
	.pre_load = (pre_load_callback*) MemScriptSection::load_script_table_from_file,
	.post_save = NULL,
	.post_load = NULL,
};