#ifndef VM_H
#define VM_H

#include "map.h"
#include "vm_bytecode.h"
#include "vm_procedure.h"
#include <stddef.h>
#include <stdint.h>

#define MAX_FRAMES 64

typedef enum {
	INTERPRET_OK,
	INTERPRET_COMPILE_ERROR,
	INTERPRET_RUNTIME_ERROR,
} InterpretResult;

typedef struct {
	Procedure *proc;
	uint8_t *ip;
} CallFrame;

typedef struct {
	CallFrame frames[MAX_FRAMES];
	size_t frame_count;

	Map *procedures;

	uint8_t *tape;
	size_t pc;
	size_t tape_len;
} Vm;

Vm *vm_new(size_t tape_len);
void vm_free(Vm *vm);
InterpretResult vm_interpret(Vm *vm, const char *source);

#endif
