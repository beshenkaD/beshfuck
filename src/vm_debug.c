#include "vm_debug.h"
#include "vm.h"
#include "vm_bytecode.h"
#include <stdio.h>

static size_t simple(const char *name, size_t offset, FILE *f)
{
	fprintf(f, "%s\n", name);
	return offset + 1;
}

static size_t call(Bytecode *bc, size_t offset, FILE *f)
{
	fprintf(f, "%s\t", "OP_CALL");
	fprintf(f, "%s\n", bc->consts.values[offset]);
	return offset + 2;
}

static size_t disassemble_opcode(Bytecode *bc, size_t offset, FILE *f)
{
	fprintf(f, "%04zu ", offset);

	uint8_t opcode = bc->code[offset];
	switch (opcode) {
	case OP_INC:
		return simple("OP_INC", offset, f);
	case OP_DEC:
		return simple("OP_DEC", offset, f);
	case OP_IN:
		return simple("OP_IN", offset, f);
	case OP_OUT:
		return simple("OP_OUT", offset, f);
	case OP_NEXT:
		return simple("OP_NEXT", offset, f);
	case OP_PREV:
		return simple("OP_PREV", offset, f);
	case OP_LSTART:
		return simple("OP_LSTART", offset, f);
	case OP_LEND:
		return simple("OP_LEND", offset, f);
	case OP_CALL:
		return call(bc, offset, f);
	}

	fprintf(f, "Unknown opcode %d\n", opcode);
	return offset + 1;
}

void vm_debug_disassemble(Bytecode *bc, const char *header, FILE *f)
{
	if (!f)
		f = stdout;

	fprintf(f, "------- %s -------\n", header);

	for (size_t offset = 0; offset < bc->count;) {
		offset = disassemble_opcode(bc, offset, f);
	}
}
