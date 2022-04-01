
#include "operators.h"

#include "process.h"

#include "primitives/stringobject.h"

string* rawdata::gets() {
  assert(dt == datatype::STR);
  return memp.str;
}

alni* rawdata::geti() {
  assert(dt == datatype::INT);
  return memp.num;
}

alnf* rawdata::getf() {
  assert(dt == datatype::FLOAT);
  return memp.flt;
}

bool* rawdata::getb() {
  assert(dt == datatype::BOOL);
  return memp.boolean;
}

// ---------------------- Instructions ------------------------- //

void createop::operator()(process* instf) {
  string* stype = instf->get_datap(type)->gets();
  Object** retp = instf->get_objaddr(ret);
  *retp = NDO->create(stype->cstr());
}

void destroyop::operator()(process* instf) {
  Object** ret = instf->get_objaddr(del);
  assert(ret && *ret);
  NDO->destroy(*ret);
}

void saveop::operator()(process* instf) {
  StringObject** spath = (StringObject**)instf->get_objaddr(path);
  Object** objp = instf->get_objaddr(obj);
  NDO_CAST_ASSERT(StringObject, *spath);
  assert(objp && *objp);
  NDO->save(*objp, (*spath)->val);
}

void loadop::operator()(process* instf) {
  StringObject** spath = (StringObject**)instf->get_objaddr(path);
  Object** objp = instf->get_objaddr(ret);
  NDO_CAST_ASSERT(StringObject, *spath);
  *objp = NDO->load((*spath)->val);
}

void setop::operator()(process* instf) {
  rawdata* datap = instf->get_datap(data);
  Object** targetp = instf->get_objaddr(target);
  assert(targetp && *targetp);

  switch (datap->dt) {
    case rawdata::datatype::INT:
      NDO->set(*targetp, *datap->memp.num);
      return;
    case rawdata::datatype::FLOAT:
      NDO->set(*targetp, *datap->memp.flt);
      return;
    case rawdata::datatype::BOOL:
      NDO->set(*targetp, (alni)(*datap->memp.boolean));
      return;
    case rawdata::datatype::STR:
      NDO->set(*targetp, *datap->memp.str);
      return;
  }
}

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