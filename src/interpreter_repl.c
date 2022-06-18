#include "interpreter_repl.h"
#include "vm.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef USE_LIBEDIT
#include <readline/history.h>
#include <readline/readline.h>

static void read(Vm *vm)
{
	for (;;) {
		char *line = readline(">>> ");
		if (!line) {
			puts("");
			break;
		}

		add_history(line);

		vm_interpret(vm, line);
		puts("");

		free(line);
	}
}
#else
static void read(Vm *vm)
{
	char line[1024];
	for (;;) {
		printf("\n> ");
		if (!fgets(line, sizeof(line), stdin)) {
			puts("");
			break;
		}

		vm_interpret(vm, line);
	}
}
#endif

void interpreter_repl()
{
	Vm *vm = vm_new(30000);
	read(vm);
	vm_free(vm);
}
