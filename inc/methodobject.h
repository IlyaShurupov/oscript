#pragma once

#include "script_section.h"

namespace obj {

	enum MethodObjectTypeMethods {
		METHOD_OBJECT_COMPILE,
		METHOD_GET_STRING_OBJECT,
	};

	struct MethodObject : Object {

		osc::script_data* script;

		static void constructor(MethodObject* self);
		static void copy(MethodObject* self, const MethodObject* in);
		static void destructor(MethodObject* self);
	};


	extern ObjectType MethodObjectType;

};