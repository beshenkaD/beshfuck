#include "interpreter.h"
#include <assert.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	if (argc == 1) {
		interpreter_repl();
	} else if (argc == 2) {
		interpreter_do_file(argv[1]);
	} else {
		return 324;
	}

	return 0;
}
