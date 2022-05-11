#pragma once

#include "bytecode.h"

#include "callstack.h"

#include "cassert.h"
#include "object/object.h"
#include "primitives/stringobject.h"

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

		case inst::itype::DESTROY:
			idestroy(cs, bc.fetch_csl());
			break;

		case inst::itype::LOAD:
			iloadop(cs, bc.fetch_csl(), bc.fetch_csl());
			break;

		case inst::itype::NONE:
			break;

		default: assert(0 && "unhandled contol flow");
	}
}


void fbody::icreate(callstack* cs, inst::iargs::cdp otype, inst::iargs::csl local_ret) {
	string* stype = cd.get<string>(otype);
	Object** retp = cs->local(local_ret);
	*retp = NDO->create(stype->cstr());
}

// struct writeblock<Type> { uint1 dt { INT = 0, FLOAT, BOOL, STRING }; Type rawdata; };
void fbody::iwrite(callstack* cs, inst::iargs::cdp writeblock, inst::iargs::csl local_write) {
	uint1* writeblock_type = cd.get<uint1>(writeblock);
	Object** targetp = cs->local(local_write);
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
	StringObject** path = (StringObject**) cs->local(local_path);
	Object** objp = cs->local(local_target);
	NDO_CAST_ASSERT(StringObject, *path);
	assert(objp && *objp);
	NDO->save(*objp, (*path)->val);
}

void fbody::idestroy(callstack* cs, inst::iargs::csl local_target) {
	Object** ret = cs->local(local_target);
	assert(ret && *ret);
	NDO->destroy(*ret);
}

void fbody::iloadop(callstack* cs, inst::iargs::csl local_ret, inst::iargs::csl local_path) {
	StringObject** path = (StringObject**) cs->local(local_path);
	Object** ret = cs->local(local_ret);
	NDO_CAST_ASSERT(StringObject, *path);
	*ret = NDO->load((*path)->val);
}


/*
void tmpushret::operator()(process* instf) { instf->pushret(ret); }

void tmpusharg::operator()(process* instf) { instf->pusharg(arg); }

void tmcallop::operator()(process* instf) {
	Object* targeto = *instf->get_objaddr(target);
	string* smethodid = instf->get_datap(methodid)->gets();
	assert(targeto);

	// replace with dictinary
	for (type_method* iter = targeto->type->methods; iter; iter++) {
		if (iter->name == *smethodid) {
			iter->adress(targeto, instf);
		}
	}
}
*/
