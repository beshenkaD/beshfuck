#ifndef VM_CONSTANTS_H
#define VM_CONSTANTS_H

#include <stddef.h>
#include <stdint.h>

typedef char *Value;

typedef struct {
	size_t count;
	size_t capacity;
	Value *values;
} Constants;

void vm_constants_init(Constants *c);
void vm_constants_push(Constants *c, Value value);
void vm_constants_free(Constants *c);

#endif
