#include "interpreter.h"
#include "vm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *read_file(const char *path)
{
	FILE *f = fopen(path, "rb");
	if (!f) {
		fprintf(stderr, "could not open file `%s`.\n", path);
		exit(10);
	}

	fseek(f, 0L, SEEK_END);
	size_t file_size = ftell(f);
	rewind(f);

	char *buffer = malloc(file_size + 1);
	if (!buffer) {
		fprintf(stderr, "not enough memory to read.\n");
		exit(10);
	}

	size_t bytes_read = fread(buffer, sizeof(char), file_size, f);
	if (bytes_read < file_size) {
		fprintf(stderr, "could not read file\n");
		exit(10);
	}

	buffer[bytes_read] = '\0';
	fclose(f);

	return buffer;
}

void interpreter_do_file(const char *path)
{
	char *source = read_file(path);

	Vm *vm = vm_new(30000);
	InterpretResult res = vm_interpret(vm, source);
	free(source);

	vm_free(vm);
	exit(res);
}

void interpreter_repl()
{
	Vm *vm = vm_new(30000);

	char line[1024];
	for (;;) {
		printf("\n> ");
		if (!fgets(line, sizeof(line), stdin)) {
			puts("\n");
			break;
		}

		vm_interpret(vm, line);
	}

	vm_free(vm);
}
