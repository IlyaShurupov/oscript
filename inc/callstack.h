#pragma once

#include "array.h"
#include "object/object.h"

typedef uint1 local_idx; // call stack local

// function frame
struct frame {
	Object* ret;
	struct fbody* func; // function code
	alni bp; // previous base pointer
	// fucntion Objects* locals //
};

class callstack {
	enum { MAX_FRAMES = 50, AVG_FRAME_SIZE = 16 };

	Array<alni> frames; // memory for frames
	alni bp; // base pointer
	alni fp; // frame pointer: bp + sizeof(frame) + sizeof(Object*) * nlocals
	uint1 npassed = 0; // objects passed so far in current frame

	public:

	callstack() {
		bp = 0;
		fp = sizeof(frame);
		frames.Reserve(MAX_FRAMES * AVG_FRAME_SIZE);
	}

	// get local object at index in current frame
	Object*& get(local_idx idx) {
		return *(((Object**) &frames[fp + sizeof(frame) / 8]) + idx);
	}

	// passes object to next frame
	void pass_to_next_frame(Object* obj) {
		local_next_frame(npassed) = obj;
		npassed++;
	}

	// enter next frame with new code
	void call(fbody* func) {
		((frame&) fp).func = func;
		((frame&) fp).bp = bp;
		bp = fp;
		fp = bp + sizeof(frame) + npassed * sizeof(Object*);
		npassed = 0;
	}

	// reserves space for locals. passed by previous frame locals already reserved.
	void reserve_locals(alni nlocals) {
		fp += sizeof(Object*) * nlocals;
	}

	// returns to previous frame
	void return_from_frame() {
		fp = bp;
		bp = ((frame&) bp).bp;
		npassed = 0;
	}

	private:
	Object*& local_next_frame(local_idx idx) {
		return *(((Object**) &frames[fp + sizeof(frame) / 8]) + idx);
	}
};