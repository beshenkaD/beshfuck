#include "vm_bytecode.h"
#include <stdlib.h>

void vm_bytecode_init(Bytecode *bc)
{
	bc->code = NULL;
	bc->count = bc->capacity = 0;
}

void vm_bytecode_push(Bytecode *bc, uint8_t opcode)
{
	if (bc->capacity < bc->count + 1) {
		bc->capacity *= 2;
		bc->code = realloc(bc->code, bc->capacity * sizeof(uint8_t));
	}

	bc->code[bc->count++] = opcode;
}

void vm_bytecode_free(Bytecode *bc)
{
	free(bc->code);
	vm_bytecode_init(bc);
}
