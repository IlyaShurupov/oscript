#pragma once

#include "bytecode.h"

#include "callstack.h"

#include "cassert.h"
#include "object/object.h"
#include "primitives/stringobject.h"

#include "primitives/dictobject.h"
#include "methodobject.h"

inst::itype bytecode::fetch_itype() {
	auto it = (inst::itype*) &buff[ip];
	ip += sizeof(inst::itype);
	return *it;
}

inst::iargs::cdp bytecode::fetch_cd() {
	auto cdp = (inst::iargs::cdp*) &buff[ip];
	ip += sizeof(inst::iargs::cdp);
	return *cdp;
}

inst::iargs::csl bytecode::fetch_csl() {
	auto csl = (inst::iargs::csl*) &buff[ip];
	ip += sizeof(inst::iargs::csl);
	return *csl;
}

void fbody::exec(callstack* cs) {
	switch (bc.fetch_itype()) {
		case inst::itype::CREATE:
			icreate(cs, bc.fetch_cd(), bc.fetch_csl());
			break;

		case inst::itype::WRITE:
			iwrite(cs, bc.fetch_cd(), bc.fetch_csl());
			break;

		case inst::itype::SAVE:
			isave(cs, bc.fetch_csl(), bc.fetch_csl());
			break;

		case inst::itype::LOAD:
			iload(cs, bc.fetch_csl(), bc.fetch_csl());
			break;

		case inst::itype::CALL:
			icall(cs, bc.fetch_cd(), bc.fetch_csl());
			break;

		case inst::itype::RESERVE:
			ireserve(cs, bc.fetch_csl());
			break;

		case inst::itype::PUSH:
			ipush(cs, bc.fetch_csl());
			break;

		case inst::itype::RET:
			iret(cs, bc.fetch_csl());
			break;

		case inst::itype::DESTROY:
			idestroy(cs, bc.fetch_csl());
			break;

		case inst::itype::MOVE:
			imove(cs, bc.fetch_csl(), bc.fetch_csl());
			break;

		case inst::itype::JUMPIF:
			ijumpif(cs, bc.fetch<int2>(), bc.fetch_csl());
			break;
			
		case inst::itype::JUMP:
			ijump(cs, bc.fetch<int2>());
			break;

		case inst::itype::JUMPIFNOT:
			ijumpifnot(cs, bc.fetch<int2>(), bc.fetch_csl());
			break;

		case inst::itype::NONE:
			break;

		default: assert(0 && "unhandled contol flow");
	}
}

void fbody::idestroy(callstack* cs, inst::iargs::csl local_del) {
	Object** retp = &cs->get(local_del);
	NDO->destroy(*retp);
}

void fbody::icreate(callstack* cs, inst::iargs::cdp otype, inst::iargs::csl local_ret) {
	string* stype = cd.get<string>(otype);
	Object** retp = &cs->get(local_ret);
	*retp = NDO->create(stype->cstr());
}

// struct writeblock<Type> { uint1 dt { INT = 0, FLOAT, BOOL, STRING }; Type rawdata; };
void fbody::iwrite(callstack* cs, inst::iargs::cdp writeblock, inst::iargs::csl local_write) {
	uint1* writeblock_type = cd.get<uint1>(writeblock);
	Object** targetp = &cs->get(local_write);
	void* rawdata = (void*) (writeblock_type + 1);

	assert(targetp && *targetp);

	switch (*writeblock_type) {
		case 0:
			NDO->set(*targetp, *(alni*) (rawdata));
			return;
		case 1:
			NDO->set(*targetp, *(alnf*) (rawdata));
			return;
		case 2:
			NDO->set(*targetp, *(alni*) (rawdata));
			return;
		case 3:
			NDO->set(*targetp, *(string*) (rawdata));
			return;

		default: assert(0 && "unhandled contol flow");
	}
}

void fbody::isave(callstack* cs, inst::iargs::csl local_target, inst::iargs::csl local_path) {
	StringObject** path = (StringObject**) &cs->get(local_path);
	Object** objp = &cs->get(local_target);
	NDO_CAST_ASSERT(StringObject, *path);
	assert(objp && *objp);
	NDO->save(*objp, (*path)->val);
}

void fbody::iload(callstack* cs, inst::iargs::csl local_ret, inst::iargs::csl local_path) {
	StringObject** path = (StringObject**) &cs->get(local_path);
	Object** ret = &cs->get(local_ret);
	NDO_CAST_ASSERT(StringObject, *path);
	*ret = NDO->load((*path)->val);
}

void fbody::ijumpif(callstack* cs, int2 offset, inst::iargs::csl local_boolean) {
	Object* obj = cs->get(local_boolean);
	assert(obj && obj->type->convesions && obj->type->convesions->to_int);
	alni val = obj->type->convesions->to_int(obj);
	if (val) {
		cs->jump(offset);
	}
}

void fbody::ijumpifnot(callstack* cs, int2 offset, inst::iargs::csl local_boolean) {
	Object* obj = cs->get(local_boolean);
	assert(obj && obj->type->convesions && obj->type->convesions->to_int);
	alni val = obj->type->convesions->to_int(obj);
	if (!val) {
		cs->jump(offset);
	}
}

void fbody::ijump(callstack* cs, int2 offset) {
	cs->jump(offset);
}

void fbody::imove(callstack* cs, inst::iargs::csl local_from, inst::iargs::csl local_to) {
	cs->get(local_to) = cs->get(local_from);
}

void fbody::icall(callstack* cs, inst::iargs::cdp callable_name, inst::iargs::csl loca_callable_self) {
	Object* targeto = cs->get(loca_callable_self);
	string* smethodid = cd.get<string>(callable_name);

	assert(targeto && smethodid);

	if (targeto->type == &DictObjectType) {
		NDO_CASTV(DictObject, targeto, dicto);
		auto method_obj_idx = dicto->items.Presents(smethodid);
		if (method_obj_idx) {
			assert(dicto->items[method_obj_idx]->type == &MethodObjectType && "cant call non method object");
			NDO_CASTV(MethodObject, dicto->items[method_obj_idx], methodo);
			cs->call(&methodo->script->code);
			return;
		}
	}

	struct tmcaller : public object_caller {

		Object* get(alni idx) override {
			return NULL;
		}

		void ret(Object*) override {

		}
	};

	tmcaller caller;

	// replace with dictinary
	for (type_method* iter = targeto->type->methods; iter; iter++) {
		if (iter->name == *smethodid) {
			iter->adress(targeto, &caller);
			return;
		}
	}

	assert(0 && "unhandled flow");
}

void fbody::ireserve(callstack* cs, uint1 n_locals) {
	cs->reserve_locals(n_locals);
}

void fbody::ipush(callstack* cs, inst::iargs::csl local_arg) {
	cs->pass_to_next_frame(cs->get(local_arg));
}

void fbody::iret(callstack* cs, inst::iargs::csl local_ret) {
	cs->return_from_frame(cs->get(local_ret));
}