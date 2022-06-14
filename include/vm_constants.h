#ifndef VM_CONSTANTS_H
#define VM_CONSTANTS_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
	size_t count;
	size_t capacity;
	char **values;
} Constants;

void vm_constants_init(Constants *c);
void vm_constants_push(Constants *c, char *val);
void vm_constants_free(Constants *c);

#endif
