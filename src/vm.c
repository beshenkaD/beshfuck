#include "vm.h"
#include "vm_bytecode.h"
#include <stdio.h>
#include <stdlib.h>

Vm *vm_new(size_t tape_len, Bytecode *bc)
{
	Vm *vm = malloc(sizeof(Vm));
	vm->bc = bc;
	vm->tape = calloc(tape_len, sizeof(int32_t));
	vm->tape_len = tape_len;
	vm->pc = 0;

	return vm;
}

void vm_free(Vm *vm)
{
	free(vm->tape);
	free(vm);
}

void vm_do(Vm *vm)
{
	size_t c = 0;

	for (size_t i = 0; i < vm->bc->count; i++) {
		switch (vm->bc->code[i]) {
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
				while (c > 0 || vm->bc->code[i] != OP_LEND) {
					if (vm->bc->code[i] == OP_LSTART)
						c++;
					else if (vm->bc->code[i] == OP_LEND)
						c--;
					i++;
				}
			}
			break;
		case OP_LEND:
			if (vm->tape[vm->pc] != 0) {
				i--;

				while (c > 0 || vm->bc->code[i] != OP_LSTART) {
					if (vm->bc->code[i] == OP_LEND)
						c++;
					else if (vm->bc->code[i] == OP_LSTART)
						c--;
					i--;
				}

				i--;
			}

			break;
		case OP_PSTART:
			break;
		case OP_PEND:
			break;
		case OP_CALL:
			break;
		case OP_SPAWN:
			break;
		default:
			break;
		}
	}
}
