#ifndef VM_BYTECODE_H
#define VM_BYTECODE_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
	uint8_t *code;
	size_t count;
	size_t capacity;
} Bytecode;

void vm_bytecode_init(Bytecode *bc);
void vm_bytecode_push(Bytecode *bc, uint8_t opcode);
void vm_bytecode_free(Bytecode *bc);

#endif
