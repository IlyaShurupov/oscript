#pragma once

#include "env.h"
#include "strings.h"

struct process;

struct rawdata {
  enum class datatype { INT, FLOAT, BOOL, STR } dt;
  union {
    alni* num;
    alnf* flt;
    bool* boolean;
    string* str;
  } memp;

  string* gets();
  alni* geti();
  alnf* getf();
  bool* getb();
};

typedef halni objaddr;
typedef halni rawdataddr;

struct createop {
  rawdataddr type;
  objaddr ret;
  void operator()(process* instf);
};

struct destroyop {
  objaddr del;
  void operator()(process* instf);
};

struct saveop {
  objaddr path;
  objaddr obj;
  void operator()(process* instf);
};

struct loadop {
  objaddr path;
  objaddr ret;
  void operator()(process* instf);
};

struct setop {
  rawdataddr data;
  objaddr target;
  void operator()(process* instf);
};

struct tmpushret {
  objaddr ret;
  void operator()(process* instf);
};

struct tmpusharg {
  objaddr arg;
  void operator()(process* instf);
};

struct tmcallop {
  objaddr target;
  rawdataddr methodid;
  void operator()(process* instf);
};
