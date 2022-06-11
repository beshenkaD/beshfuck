#include "compiler.h"
#include "vm.h"
#include "vm_bytecode.h"
#include <assert.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	assert(argc >= 2);

	FILE *src = fopen(argv[1], "rb");
	assert(src);

	Bytecode bc = compiler_do_file(src);

	Vm *vm = vm_new(1000 * 30, &bc);

	vm_do(vm);

	fclose(src);
	vm_bytecode_free(&bc);
	vm_free(vm);

	return 0;
}
