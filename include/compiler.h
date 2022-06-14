#ifndef COMPILER_H
#define COMPILER_H

#include "stdbool.h"
#include "stdio.h"
#include "vm.h"
#include "vm_bytecode.h"
#include "vm_procedure.h"

Procedure *compiler_do(Vm *vm, const char *source);

#endif
