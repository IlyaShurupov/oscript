#pragma once

#include "array.h"
#include "object/object.h"

struct callstack {
	Array<alni> buff;

	typedef uint1 fli; // frame local idx
	Object** local(fli local) {
		return (Object**) &buff[local];
	}
};