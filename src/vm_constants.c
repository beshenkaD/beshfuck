#include "vm_constants.h"
#include <stdarg.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 32

void vm_constants_init(Constants *c)
{
	c->values = malloc(INITIAL_CAPACITY * sizeof(Value));
	c->count = 0;
	c->capacity = INITIAL_CAPACITY;
}

void vm_constants_push(Constants *c, Value value)
{
	if (c->capacity == c->count) {
		c->capacity *= 2;
		c->values = realloc(c->values, c->capacity * sizeof(Value));
	}

	c->values[c->count++] = value;
}

void vm_constants_free(Constants *c)
{
	free(c->values);
	c->values = NULL;
	c->count = c->capacity = 0;
}
