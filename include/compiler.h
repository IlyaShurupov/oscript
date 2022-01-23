#pragma once

#include "vm.h"

void osc_init();
void osc_compile(code* out, string* oscript);
void osc_finalize();