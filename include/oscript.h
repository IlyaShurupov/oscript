#pragma once

#include "vm.h"

#include "optable.h"
#include "primitives/primitives.h"

#include "methodobject.h"
#include "classobject.h"

#include "script_section.h"

objects_api* oscript_init() {
	
	objects_api* oh = objects_init();

	primitives_define_types(oh);

	NDO->define(&MethodObjectType);
	NDO->define(&ClassObjectType);

	NDO->add_sl_callbacks(&slcb_script_section);

	return oh;
}