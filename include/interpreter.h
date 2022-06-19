#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "interpreter_repl.h"
#include <stddef.h>

void interpreter_do_file(const char *path, size_t tape_len);

#endif
