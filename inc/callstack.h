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

	inline Object*& local(local_idx idx) {
		return (((Object**)&bp)[idx]);
	}
};

class callstack {
	enum { MAX_FRAMES = 50, AVG_FRAME_SIZE = 16 };

	Array<alni> frames; // memory for frames
	alni bp; // base pointer
	alni fp; // frame pointer: bp + sizeof(frame) + sizeof(Object*) * nlocals
	uint1 npassed = 0; // objects passed so far in current frame

	public:

	callstack();

	// get local object at index in current frame
	Object*& get(local_idx idx);

	// passes object to next frame
	void pass_to_next_frame(Object* obj);

	// enter next frame with new code
	void call(fbody* func);

	// reserves space for locals. passed by previous frame locals already reserved.
	void reserve_locals(alni nlocals);

	// returns to previous frame
	void return_from_frame(Object* ret);

	void jump(int2 offset);

	private:
	Object*& local_next_frame(local_idx idx);
	frame* current_frame();
	frame* next_frame();
};