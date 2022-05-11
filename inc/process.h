#pragma once

#include "callstack.h"
#include "bytecode.h"

struct process {
  callstack cs;
  fbody* fb;

  void run() {
    do {
      fb->exec(&cs);
    } while (fb->bc.ip < fb->bc.buff.length);
  }
};