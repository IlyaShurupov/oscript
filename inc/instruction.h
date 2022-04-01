#pragma once

#include "operators.h"

enum class operator_code {
  NONE,
  CREATE,
  DESTROY,
  SAVE,
  LOAD,
  SET,
  CALL,
};

struct instruction {

  union {
    createop create;
    destroyop destroy;
    saveop save;
    loadop load;
    setop set;
    tmcallop mtcall;
  } inst;

  alni opcode;

  // 2 alni for instruction info
  instruction(operator_code opcode);
  void execute(process* iframe);
};