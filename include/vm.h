#ifndef VM_H
#define VM_H

#include "vm_bytecode.h"
#include "vm_procedures.h"
#include <stddef.h>
#include <stdint.h>

enum {
	/*Name     Description                           Args       */
	OP_INC,	   /*  Increment                                -   */
	OP_DEC,	   /*  Decrement                                -   */
	OP_IN,	   /*  Input                                    -   */
	OP_OUT,	   /*  Output                                   -   */
	OP_NEXT,   /*  Next cell                                -   */
	OP_PREV,   /*  Previous cell                            -   */
	OP_LSTART, /*  Loop start                               -   */
	OP_LEND,   /*  Loop end                                 -   */
	OP_CALL,
	OP_SPAWN,
};

typedef struct {
	Bytecode *bc;
	Procedures procs;

	int32_t *tape;
	size_t pc;
	size_t tape_len;
} Vm;

Vm *vm_new(size_t tape_len, Bytecode *bc);
void vm_free(Vm *vm);
void vm_do(Vm *vm);
void vm_do_bytecode(Vm *vm, Bytecode *bc);
void test();

#endif
