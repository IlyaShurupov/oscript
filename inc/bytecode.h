#pragma once

#include "array.h"

class callstack;

// oscript instruction
namespace inst {
	// instruction type
	enum class itype {
		NONE,
		CREATE,
		SAVE,
		LOAD,
		WRITE,
		CALL,
		RESERVE,
		PUSH,
		RET,
		DESTROY,
	};

	// instruction argumentgs
	namespace iargs {
		typedef uint4 cdp; // const data pointer
		typedef uint1 csl; // call stack local
	};
};

// const data sector
struct constdata {
	uint1* mem = NULL;

	template <typename Type>
	Type* get(inst::iargs::cdp idx) {
		assert(mem);
		return (Type*) (mem + idx);
	}
};

// code sector
struct bytecode {
	Array<uint1> buff;

	alni ip = 0; // instruction pointer

	inst::itype fetch_itype();
	inst::iargs::cdp fetch_cd();
	inst::iargs::csl fetch_csl();
};

// compilation unit
struct fbody {

	constdata cd;
	bytecode bc;

	// executes instruction at ip
	void exec(callstack* cs);

	// instruction implementations

	void icreate(callstack* cs, inst::iargs::cdp otype, inst::iargs::csl local_ret);
	void iwrite(callstack* cs, inst::iargs::cdp writeblock, inst::iargs::csl local_write);
	void isave(callstack* cs, inst::iargs::csl local_target, inst::iargs::csl local_path);
	void iload(callstack* cs, inst::iargs::csl local_ret, inst::iargs::csl local_path);
	void idestroy(callstack* cs, inst::iargs::csl local_rem);

	void ipush(callstack* cs, inst::iargs::csl local_arg);
	void ireserve(callstack* cs, uint1 n_locals);
	void icall(callstack* cs, inst::iargs::cdp callable_name, inst::iargs::csl local_callable_self);
	void iret(callstack* cs, inst::iargs::csl local_ret, inst::iargs::csl ret_adress);
};
