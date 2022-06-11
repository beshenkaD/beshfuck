#ifndef COMPILER_H
#define COMPILER_H

#include "stdio.h"
#include "vm_bytecode.h"

Bytecode compiler_do_file(FILE *f);
Bytecode compiler_do_string(const char *s);

#endif
