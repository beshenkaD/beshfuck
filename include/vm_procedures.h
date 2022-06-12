#ifndef VM_STATE_H
#define VM_STATE_H

#include "vm_bytecode.h"

// TODO: make this thing a hash table
typedef struct {
	size_t count;
	size_t capacity;
	char **names;
	Bytecode *procs;
} Procedures;

void vm_procedures_init(Procedures *s);
void vm_procedures_add(Procedures *s, char *name, Bytecode bc);
Bytecode vm_procedures_get(Procedures *s, char *name);
void vm_procedures_free(Procedures *s);

#endif
