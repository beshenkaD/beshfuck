#include "compiler.h"

#include "vm_bytecode.h"
#include "vm_debug.h"
#include "vm_procedure.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Procedure *compiler_do(Vm *vm, const char *source)
{
	assert(source);
	assert(vm);

	Procedure *main = vm_procedure_new();

	// parsing here :)

	vm_bytecode_push(&main->bc, OP_RETURN);

	return main;
}
