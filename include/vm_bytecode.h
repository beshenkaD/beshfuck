#ifndef VM_BYTECODE_H
#define VM_BYTECODE_H

#include "vm_constants.h"
#include <stddef.h>
#include <stdint.h>

typedef enum {
	/*Name     Description                           Args       */
	OP_INC,	   /*  Increment                                -   */
	OP_DEC,	   /*  Decrement                                -   */
	OP_IN,	   /*  Input                                    -   */
	OP_OUT,	   /*  Output                                   -   */
	OP_NEXT,   /*  Next cell                                -   */
	OP_PREV,   /*  Previous cell                            -   */
	OP_LSTART, /*  Loop start                               -   */
	OP_LEND,   /*  Loop end                                 -   */
	OP_CALL,   /*  Calls procedure                     Constant */
	OP_RETURN, /*  Return from procedure                        */
	OP_SPAWN,  /*  Unknown                                      */
	OP_LOAD,   /*  Load constant into memory           Constant */
} OpCodes;

typedef struct {
	size_t count;
	size_t capacity;
	uint8_t *code;
	Constants constants;
} Bytecode;

void vm_bytecode_init(Bytecode *bc);
void vm_bytecode_push(Bytecode *bc, uint8_t opcode);
void vm_bytecode_free(Bytecode *bc);
size_t vm_bytecode_add_constant(Bytecode *bc, char *val);

#endif
