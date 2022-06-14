#include "vm_procedure.h"
#include "vm_bytecode.h"
#include <stdlib.h>

Procedure *vm_procedure_new()
{
	Procedure *p = malloc(sizeof(Procedure));
	vm_bytecode_init(&p->bc);
	p->name = NULL;

	return p;
}

void vm_procedure_free(Procedure *p)
{
	vm_bytecode_free(&p->bc);
	free(p);
}
