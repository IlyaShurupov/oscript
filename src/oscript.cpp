
#include "oscript.h"

objects_api* oscript_init() {

	objects_api* oh = objects_init();

	primitives_define_types(oh);

	NDO->define(&MethodObjectType);
	NDO->define(&ClassObjectType);

	NDO->add_sl_callbacks(&slcb_script_section);

	osc_init();

	return oh;
}