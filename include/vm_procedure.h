#ifndef PROCEDURE_H
#define PROCEDURE_H

#include "vm_bytecode.h"

typedef struct {
	Bytecode bc;
	uint8_t *ip;
	char *name;
} Procedure;

Procedure *vm_procedure_new(char *name);
void vm_procedure_free(Procedure *p);

#endif
