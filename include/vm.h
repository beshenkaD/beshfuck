#ifndef VM_H
#define VM_H

#include "vm_bytecode.h"
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
	OP_PSTART, /*  Procedure start               Procedure name */
	OP_PEND,   /*  Procedure end                            -   */
	OP_CALL,   /*  Call procedure                Procedure name */
	OP_SPAWN,  /*  Call procedure in new thread  Procedure name */
} OpCodes;

typedef struct {
	Bytecode bc;

	int32_t *tape;
	size_t pc;
	size_t tape_len;
} Vm;

Vm *vm_new(size_t tape_len, Bytecode bc);
void vm_delete(Vm *vm);
void vm_do(Vm *vm);
void test();

#endif
