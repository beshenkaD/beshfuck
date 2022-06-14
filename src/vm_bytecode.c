#include "vm_bytecode.h"
#include "vm_constants.h"
#include <stdlib.h>

#define INITIAL_CAPACITY 32

void vm_bytecode_init(Bytecode *bc)
{
	bc->code = malloc(INITIAL_CAPACITY * sizeof(uint8_t));
	bc->count = 0;
	bc->capacity = INITIAL_CAPACITY;
	vm_constants_init(&bc->constants);
}

void vm_bytecode_push(Bytecode *bc, uint8_t opcode)
{
	if (bc->capacity == bc->count) {
		bc->capacity *= 2;
		bc->code = realloc(bc->code, bc->capacity * sizeof(uint8_t));
	}

	bc->code[bc->count++] = opcode;
}

size_t vm_bytecode_add_constant(Bytecode *bc, char *val)
{
	vm_constants_push(&bc->constants, val);
	return bc->constants.count - 1;
}

void vm_bytecode_free(Bytecode *bc)
{
	free(bc->code);
	bc->code = NULL;
	bc->count = bc->capacity = 0;
	vm_constants_free(&bc->constants);
}
