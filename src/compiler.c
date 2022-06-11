#include "compiler.h"

#include "vm.h"
#include <assert.h>
#include <bits/stdint-intn.h>
#include <bits/stdint-uintn.h>
#include <stdio.h>
#include <string.h>

static int8_t do_char(char c)
{
	switch (c) {
	case '+':
		return OP_INC;
	case '-':
		return OP_DEC;
	case '>':
		return OP_NEXT;
	case '<':
		return OP_PREV;
	case '[':
		return OP_LSTART;
	case ']':
		return OP_LEND;
	case '.':
		return OP_OUT;
	case ',':
		return OP_IN;
	}

	return -1;
}

Bytecode compiler_do_file(FILE *f)
{
	assert(f);

	Bytecode bc;
	vm_bytecode_init(&bc);

	char c;
	while ((c = fgetc(f)) != EOF) {
		int8_t o = do_char(c);
		if (o != -1)
			vm_bytecode_push(&bc, (uint8_t)o);
	}

	return bc;
}

Bytecode compiler_do_string(const char *s)
{
	assert(s);

	Bytecode bc;
	vm_bytecode_init(&bc);

	for (size_t i = 0; i < strlen(s); i++) {
		int8_t o = do_char(s[i]);
		if (o != -1)
			vm_bytecode_push(&bc, (uint8_t)o);
	}

	return bc;
}
