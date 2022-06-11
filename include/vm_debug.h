#ifndef VM_DEBUG_H
#define VM_DEBUG_H

#include "stdio.h"
#include "vm_bytecode.h"

void vm_debug_disassemble(Bytecode *bc, const char *header, FILE *f);

#endif
