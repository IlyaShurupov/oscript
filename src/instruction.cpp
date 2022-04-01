#pragma once

#include "instruction.h"

#include "cassert.h"
#include "object/object.h"
#include "primitives/stringobject.h"

// -------------- implementations -------------- //

instruction::instruction(operator_code icode) : inst() {
  static_assert(sizeof(instruction) <= sizeof(alni) * 2, "size of instruction must be <= 128 biits");
  this->opcode = (halni)icode;
}

void instruction::execute(process* iframe) {
  // should be O(1)
  switch ((operator_code)opcode) {
    case operator_code::NONE:
      break;
    case operator_code::CREATE:
      inst.create(iframe);
      break;
    case operator_code::DESTROY:
      inst.destroy(iframe);
      break;
    case operator_code::SAVE:
      inst.save(iframe);
      break;
    case operator_code::LOAD:
      inst.load(iframe);
      break;
    case operator_code::SET:
      inst.set(iframe);
      break;
    case operator_code::CALL:
      inst.mtcall(iframe);
      break;
  }
}