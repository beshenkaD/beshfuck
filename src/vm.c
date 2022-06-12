#include "vm.h"
#include "vm_bytecode.h"
#include "vm_procedures.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Vm *vm_new(size_t tape_len, Bytecode *bc)
{
	Vm *vm = malloc(sizeof(Vm));
	vm->bc = bc;
	vm->tape = calloc(tape_len, sizeof(int32_t));
	vm->tape_len = tape_len;
	vm->pc = 0;

	vm_procedures_init(&vm->procs);

	return vm;
}

void vm_free(Vm *vm)
{
	vm_procedures_free(&vm->procs);
	free(vm->tape);
	free(vm);
}

static void call(Vm *vm, size_t i)
{
	Bytecode p = vm_procedures_get(&vm->procs, vm->bc->consts.values[i++]);
	assert(p.code);

	vm_do_bytecode(vm, &p);
}

// TODO
static void spawn(Vm *vm, size_t i)
{
	assert(vm);
	assert(i);
}

void vm_do_bytecode(Vm *vm, Bytecode *bc)
{
	size_t c = 0;

	for (size_t i = 0; i < bc->count; i++) {
		switch (bc->code[i]) {
		case OP_INC:
			vm->tape[vm->pc]++;
			break;
		case OP_DEC:
			vm->tape[vm->pc]--;
			break;
		case OP_IN:
			vm->tape[vm->pc] = getchar();
			break;
		case OP_OUT:
			putchar(vm->tape[vm->pc]);
			break;
		case OP_NEXT:
			vm->pc++;
			break;
		case OP_PREV:
			vm->pc--;
			break;
		case OP_LSTART:
			if (vm->tape[vm->pc] == 0) {
				i++;
				while (c > 0 || bc->code[i] != OP_LEND) {
					if (bc->code[i] == OP_LSTART)
						c++;
					else if (bc->code[i] == OP_LEND)
						c--;
					i++;
				}
			}
			break;
		case OP_LEND:
			if (vm->tape[vm->pc] != 0) {
				i--;

				while (c > 0 || bc->code[i] != OP_LSTART) {
					if (bc->code[i] == OP_LEND)
						c++;
					else if (bc->code[i] == OP_LSTART)
						c--;
					i--;
				}

				i--;
			}

			break;
		case OP_CALL:
			call(vm, i);
			i++;
			break;
		case OP_SPAWN:
			spawn(vm, i);
			i++;
			break;
		default:
			break;
		}
	}
}

void vm_do(Vm *vm)
{
	vm_do_bytecode(vm, vm->bc);
}
