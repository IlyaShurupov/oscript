#pragma once

#include "vm.h"

#include "optable.h"
#include "primitives/primitives.h"

#include "methodobject.h"
#include "classobject.h"

#include "script_section.h"

#include "compiler.h"

objects_api* oscript_init();
void oscript_finilize();