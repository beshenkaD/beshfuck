#include "vm.h"
#include "compiler.h"
#include <stdarg.h>
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

static void runtime_error(Vm *vm, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	fprintf(stderr, "[ Runtime error ]: ");
	vfprintf(stderr, fmt, args);
	fputs("\n", stderr);

	va_end(args);
	vm->frame_count = 0;
}

#define READ_BYTE() (*frame->ip++)
#define READ_SHORT() (frame->ip += 2, (uint16_t)((frame->ip[-2] << 8) | frame->ip[-1]))
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
#ifdef ENABLE_BOUNDS_CHECK
			if (vm->pc + 1 == vm->tape_len) {
				runtime_error(vm, "invalid next: out of bounds");
				return INTERPRET_RUNTIME_ERROR;
			}
#endif

			vm->pc++;
			break;
		case OP_PREV:
#ifdef ENABLE_BOUNDS_CHECK
			if (vm->pc == 0) {
				runtime_error(vm, "invalid prev: out of bounds");
				return INTERPRET_RUNTIME_ERROR;
			}
#endif

			vm->pc--;
			break;
		case OP_IN:
			vm->tape[vm->pc] = getchar();
			break;
		case OP_OUT:
			putchar(vm->tape[vm->pc]);
			break;
		case OP_OUT_DEC:
			printf("%d", vm->tape[vm->pc]);
			break;
		case OP_JUMP_IF_ZERO: {
			uint16_t offset = READ_SHORT();
			if (vm->tape[vm->pc] == 0)
				frame->ip += offset;
			break;
		}
		case OP_LOOP: {
			uint16_t offset = READ_SHORT();
			frame->ip -= offset;
			break;
		}
		case OP_CALL: {
			char *name = READ_CONST();
			Procedure *p = (Procedure *)map_get(vm->procedures, name);

			if (!p) {
				runtime_error(vm, "undefined procedure `%s`", name);
				return INTERPRET_RUNTIME_ERROR;
			}

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
		case OP_LOAD: {
			// TODO: replace atoi with something less error prone
			char *name = READ_CONST();
			int num = atoi(name);

			if (num > UINT8_MAX || num < 0) {
				runtime_error(vm, "invalid number literal `%s`", name);
				return INTERPRET_RUNTIME_ERROR;
			}

			vm->tape[vm->pc] = (uint8_t)num;

			break;
		}
		}
	}

	return INTERPRET_RUNTIME_ERROR;
}

#undef READ_BYTE
#undef READ_SHORT
#undef READ_CONST

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
