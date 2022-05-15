
#include "treecode.h"

using namespace oscript;

void irep_error::ensure(bool expr, const string& def) const {
	if (!expr) {
		rterror((string("oscript parser error - ") + def).cstr());
	}
}

// at this point we have full treecode constucted and ready to generate the actual code
void oscript::treecode::evaluate(fbody*) {

}