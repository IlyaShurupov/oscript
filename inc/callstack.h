#pragma once

#include "array.h"
#include "object/object.h"

namespace osc {

	typedef tp::uint1 local_idx; // call stack local

	// function frame
	struct frame {
		obj::Object* ret;
		struct fbody* func; // function code
		tp::alni bp; // previous base pointer
		// fucntion Objects* locals //

		inline obj::Object*& local(local_idx idx) {
			return (((obj::Object**) &bp)[idx]);
		}
	};

	class callstack {
		enum { MAX_FRAMES = 50, AVG_FRAME_SIZE = 16 };

		tp::Array<tp::alni> frames; // memory for frames
		tp::alni bp; // base pointer
		tp::alni fp; // frame pointer: bp + sizeof(frame) + sizeof(Object*) * nlocals
		tp::uint1 npassed = 0; // objects passed so far in current frame

		public:

		callstack();

		// get local object at index in current frame
		obj::Object*& get(local_idx idx);

		// passes object to next frame
		void pass_to_next_frame(obj::Object* obj);

		// enter next frame with new code
		void call(fbody* func);

		// reserves space for locals. passed by previous frame locals already reserved.
		void reserve_locals(tp::alni nlocals);

		// returns to previous frame
		void return_from_frame(obj::Object* ret);

		void jump(tp::int2 offset);

		private:
		obj::Object*& local_next_frame(local_idx idx);
		frame* current_frame();
		frame* next_frame();
	};

};