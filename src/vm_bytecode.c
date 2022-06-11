#include "vm_bytecode.h"
#include <stdlib.h>

#define INITIAL_CAPACITY 32

void vm_bytecode_init(Bytecode *bc)
{
	bc->code = malloc(INITIAL_CAPACITY * sizeof(uint8_t));
	bc->count = 0;
	bc->capacity = INITIAL_CAPACITY;
}

void vm_bytecode_push(Bytecode *bc, uint8_t opcode)
{
	if (bc->capacity == bc->count) {
		bc->capacity *= 2;
		bc->code = realloc(bc->code, bc->capacity * sizeof(uint8_t));
	}

	bc->code[bc->count++] = opcode;
}

void vm_bytecode_free(Bytecode *bc)
{
	free(bc->code);
	bc->code = NULL;
	bc->count = bc->capacity = 0;
}
