#include "interpreter.h"
#include <assert.h>
#include <stdio.h>

static void usage()
{
	fprintf(stderr, "usage: beshfuck [option] ... [file | -]\n");
}

// TODO: options parsing
int main(int argc, char *argv[])
{
	if (argc == 1) {
		interpreter_repl();
	} else if (argc == 2) {
		interpreter_do_file(argv[1]);
	} else {
		usage();
		return 324;
	}

	return 0;
}
