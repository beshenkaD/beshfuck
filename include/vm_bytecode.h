#ifndef VM_BYTECODE_H
#define VM_BYTECODE_H

#include "vm_constants.h"
#include <stddef.h>
#include <stdint.h>

typedef struct {
	size_t count;
	size_t capacity;
	uint8_t *code;
	Constants consts;
} Bytecode;

void vm_bytecode_init(Bytecode *bc);
void vm_bytecode_push(Bytecode *bc, uint8_t opcode);
void vm_bytecode_free(Bytecode *bc);
size_t vm_bytecode_add_const(Bytecode *bc, Value val);

#endif
