#include "vm_constants.h"
#include <stdlib.h>

#define INITIAL_CAPACITY 32

void vm_constants_init(Constants *c)
{
	c->values = malloc(INITIAL_CAPACITY * sizeof(uint8_t));
	c->count = 0;
	c->capacity = INITIAL_CAPACITY;
}

void vm_constants_push(Constants *c, char *val)
{
	if (c->capacity == c->count) {
		c->capacity *= 2;
		c->values = realloc(c->values, c->capacity * sizeof(char *));
	}

	c->values[c->count++] = val;
}

void vm_constants_free(Constants *c)
{
	for (size_t i = 0; i < c->count; i++) {
		free(c->values[i]);
	}

	free(c->values);
	c->values = NULL;
	c->count = c->capacity = 0;
}
