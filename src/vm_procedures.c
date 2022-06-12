#include "vm_procedures.h"
#include "vm_bytecode.h"
#include "vm_constants.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void vm_procedures_init(Procedures *s)
{
	s->names = malloc(32 * sizeof(char *));
	s->procs = malloc(32 * sizeof(Bytecode));
	s->capacity = 32;
	s->count = 0;
}

void vm_procedures_add(Procedures *s, char *name, Bytecode bc)
{
	if (s->capacity == s->count) {
		s->capacity *= 2;
		s->names = realloc(s->names, s->capacity * sizeof(char *));
		s->procs = realloc(s->procs, s->capacity * sizeof(Bytecode));
	}

	s->names[s->count] = name;
	s->procs[s->count] = bc;
	s->count++;
}

Bytecode vm_procedures_get(Procedures *s, char *name)
{
	for (size_t i = 0; i < s->count; i++) {
		if (strcmp(s->names[i], name) == 0) {
			return s->procs[i];
		}
	}

	Bytecode r;
	r.code = NULL;
	r.capacity = r.count = 0;

	return r;
}

void vm_procedures_free(Procedures *s)
{
	free(s->names);
	free(s->procs);
	s->capacity = s->count = 0;
}
