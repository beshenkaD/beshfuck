#include "vm_debug.h"
#include "vm.h"
#include "vm_bytecode.h"
#include <stdio.h>

static size_t simple(const char *name, size_t offset)
{
	printf("%s\n", name);
	return offset + 1;
}

static size_t operand(Bytecode *bc, const char *name, size_t offset)
{
	uint8_t constant = bc->code[offset + 1];
	printf("%s\t%s\n", name, bc->constants.values[constant]);
	return offset + 2;
}

static size_t jump(Bytecode *bc, const char *name, size_t offset, int sgn)
{
	uint16_t jmp = (uint16_t)((bc->code[offset + 1] << 8) | bc->code[offset + 2]);
	printf("%s %4zu -> %zu\n", name, offset, offset + 3 + sgn * jmp);
	return offset + 3;
}

static size_t disassemble_opcode(Bytecode *bc, size_t offset)
{
	printf("%04zu ", offset);

	uint8_t opcode = bc->code[offset];
	switch (opcode) {
	case OP_INC:
		return simple("OP_INC", offset);
	case OP_DEC:
		return simple("OP_DEC", offset);
	case OP_IN:
		return simple("OP_IN", offset);
	case OP_OUT:
		return simple("OP_OUT", offset);
	case OP_OUT_DEC:
		return simple("OP_OUT_DEC", offset);
	case OP_NEXT:
		return simple("OP_NEXT", offset);
	case OP_PREV:
		return simple("OP_PREV", offset);
	case OP_CALL:
		return operand(bc, "OP_CALL", offset);
	case OP_RETURN:
		return simple("OP_RETURN", offset);
	case OP_LOAD:
		return operand(bc, "OP_LOAD", offset);
	case OP_JUMP_IF_ZERO:
		return jump(bc, "OP_JUMP_IF_ZERO", offset, 1);
	case OP_LOOP:
		return jump(bc, "OP_LOOP", offset, -1);
	}

	printf("Unknown opcode %d\n", opcode);
	return offset + 1;
}

void vm_debug_disassemble(Bytecode *bc, const char *header)
{
	printf("------- %s -------\n", header);

	for (size_t offset = 0; offset < bc->count;) {
		offset = disassemble_opcode(bc, offset);
	}
}
