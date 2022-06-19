#include "interpreter.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void usage()
{
	fprintf(stderr, "usage: beshfuck [ file | - ]\n"
			"Environment variables:\n"
			"\t BESHFUCK_TAPE_LEN: use given tape length.\n");
}

int main(int argc, char *argv[])
{
	size_t tape_len = 30000;

	char *raw_len = getenv("BESHFUCK_TAPE_LEN");
	if (raw_len) {
		if (sscanf(raw_len, "%zu", &tape_len) != 1)
			fprintf(stderr, "invalid value passed to `BESHFUCK_TAPE_LEN`. Using default value: %zu\n", tape_len);
	}

	if (argc == 1) {
		interpreter_repl(tape_len);
	} else if (argc == 2) {
		if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
			usage();
			return 0;
		}

		interpreter_do_file(argv[1], tape_len);
	} else {
		usage();
		return 324;
	}

	return 0;
}
