
#include "callstack.h"

#include "bytecode.h"

namespace osc {

	callstack::callstack() {
		bp = 0;
		fp = sizeof(frame);
		frames.reserve(MAX_FRAMES * AVG_FRAME_SIZE);
	}

	// get local object at index in current frame
	obj::Object*& callstack::get(local_idx idx) {
		return current_frame()->local(idx);
	}

	// passes object to next frame
	void callstack::pass_to_next_frame(obj::Object* obj) {
		local_next_frame(npassed) = obj;
		npassed++;
	}

	// enter next frame with new code
	void callstack::call(fbody* func) {
		frame* const nframe = next_frame();
		nframe->func = func;
		nframe->bp = bp;
		bp = fp;
		fp = bp + sizeof(frame) + npassed * sizeof(obj::Object*);
		npassed = 0;
	}

	// reserves space for locals. passed by previous frame locals already reserved.
	void callstack::reserve_locals(tp::alni nlocals) {
		fp += sizeof(obj::Object*) * nlocals;
	}

	// returns to previous frame
	void callstack::return_from_frame(obj::Object* ret) {
		next_frame()->ret = ret;
		fp = bp;
		bp = current_frame()->bp;
		npassed = 0;
	}

	void callstack::jump(tp::int2 offset) {
		current_frame()->func->bc.ip += offset;
	}

	obj::Object*& callstack::local_next_frame(local_idx idx) {
		return next_frame()->local(idx);
	}

	frame* callstack::current_frame() {
		return ((frame*) &frames[bp]);
	}

	frame* callstack::next_frame() {
		return ((frame*) &frames[fp]);
	}

};