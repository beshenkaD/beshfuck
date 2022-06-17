#include "vm.h"
#include "compiler.h"
#include "map.h"
#include "vm_bytecode.h"
#include "vm_procedure.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

Vm *vm_new(size_t tape_len)
{
	Vm *vm = malloc(sizeof(Vm));
	vm->tape = calloc(tape_len, sizeof(uint8_t));
	vm->tape_len = tape_len;
	vm->pc = 0;

	vm->frame_count = 0;
	vm->procedures = map_new();

	return vm;
}

void vm_free(Vm *vm)
{
	MapIter it = map_iterator(vm->procedures);

	while (map_iterator_next(&it)) {
		vm_procedure_free(it.value);
	}

	map_free(vm->procedures);
	free(vm->tape);
	free(vm);
}

static void call(Vm *vm, Procedure *p)
{
	CallFrame *frame = &vm->frames[vm->frame_count++];
	frame->proc = p;
	frame->ip = p->bc.code;
}

#define READ_BYTE() (*frame->ip++)
#define READ_CONST() (frame->proc->bc.constants.values[READ_BYTE()])

static InterpretResult run(Vm *vm)
{
	CallFrame *frame = &vm->frames[vm->frame_count - 1];

	for (;;) {
		uint8_t instruction = READ_BYTE();
		switch (instruction) {
		case OP_INC:
			vm->tape[vm->pc]++;
			break;
		case OP_DEC:
			vm->tape[vm->pc]--;
			break;
		case OP_NEXT:
			vm->pc++;
			break;
		case OP_PREV:
			vm->pc--;
			break;
		case OP_IN:
			vm->tape[vm->pc] = getchar();
			break;
		case OP_OUT:
			putchar(vm->tape[vm->pc]);
			break;
		case OP_LSTART:
			// Use jumps?
			break;
		case OP_LEND:
			break;
		case OP_CALL: {
			Procedure *p = (Procedure *)map_get(vm->procedures, READ_CONST());
			assert(p);

			call(vm, p);

			frame = &vm->frames[vm->frame_count - 1];
			break;
		}
		case OP_RETURN:
			vm->frame_count--;

			if (vm->frame_count == 0)
				return INTERPRET_OK;

			frame = &vm->frames[vm->frame_count - 1];
			break;
		}
	}

	return INTERPRET_OK;
}

InterpretResult vm_interpret(Vm *vm, const char *source)
{
	Procedure *main = compiler_do(vm, source);

	if (!main)
		return INTERPRET_COMPILE_ERROR;

	CallFrame *frame = &vm->frames[vm->frame_count++];
	frame->proc = main;
	frame->ip = main->bc.code;

	InterpretResult r = run(vm);

	vm_procedure_free(main);

	return r;
}

// void vm_do_bytecode(Vm *vm, Bytecode *bc)
// {
// 	size_t c = 0;

// 	for (size_t i = 0; i < bc->count; i++) {
// 		switch (bc->code[i]) {
// 		case OP_INC:
// 			vm->tape[vm->pc]++;
// 			break;
// 		case OP_DEC:
// 			vm->tape[vm->pc]--;
// 			break;
// 		case OP_IN:
// 			vm->tape[vm->pc] = getchar();
// 			break;
// 		case OP_OUT:
// 			putchar(vm->tape[vm->pc]);
// 			break;
// 		case OP_NEXT:
// 			vm->pc++;
// 			break;
// 		case OP_PREV:
// 			vm->pc--;
// 			break;
// 		case OP_LSTART:
// 			if (vm->tape[vm->pc] == 0) {
// 				i++;
// 				while (c > 0 || bc->code[i] != OP_LEND) {
// 					if (bc->code[i] == OP_LSTART)
// 						c++;
// 					else if (bc->code[i] == OP_LEND)
// 						c--;
// 					i++;
// 				}
// 			}
// 			break;
// 		case OP_LEND:
// 			if (vm->tape[vm->pc] != 0) {
// 				i--;

// 				while (c > 0 || bc->code[i] != OP_LSTART) {
// 					if (bc->code[i] == OP_LEND)
// 						c++;
// 					else if (bc->code[i] == OP_LSTART)
// 						c--;
// 					i--;
// 				}

// 				i--;
// 			}

// 			break;
// 		case OP_CALL:
// 			break;
// 		case OP_SPAWN:
// 			break;
// 		default:
// 			break;
// 		}
// 	}
// }
