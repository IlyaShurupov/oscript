#pragma once

#include "array.h"
#include "instruction.h"
#include "object/object.h"

#define CALL_STACK_SIZE 100

struct call_stack {
  alni mem[CALL_STACK_SIZE]{};
  alni bp, fp = 0;
};

struct code : public Array<instruction> {};

struct process : public object_caller {
  bool finished = false;
  alni finish_state = 0;
  call_stack cstack;
  code* fbody;
  alni ip = 0;

  process() {}

  Object** get_objaddr(objaddr addr) {
    return (Object**)&cstack.mem[cstack.bp + addr];
  }
  rawdata* get_datap(rawdataddr addr) {
    return (rawdata*)cstack.mem[cstack.bp + addr];
  }

  void pusharg(objaddr addr) {}
  void pushret(objaddr retaddr) {}

  void run_instruction() {
    if (fbody->Len() <= ip) {
      finished = true;
      finish_state = 1;
      return;
    }

    (*fbody)[ip].execute(this);
    ip++;
  }
};