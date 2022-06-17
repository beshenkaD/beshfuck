#include "compiler.h"

#include "lexer.h"
#include "map.h"
#include "vm_bytecode.h"
#include "vm_debug.h"
#include "vm_procedure.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	Token current;
	Token previous;
	bool had_error;
} Parser;

Parser parser;

static void advance()
{
	parser.previous = parser.current;

	for (;;) {
		parser.current = scan_token();
		if (parser.current.type != TK_ERROR)
			break;

		printf("error token\n");
	}
}

static void error_at(Token *tk, const char *msg)
{
	fprintf(stderr, "Error");
	fprintf(stderr, " at `%.*s`", (int)tk->length, tk->start);
	fprintf(stderr, ": %s\n", msg);

	parser.had_error = true;
}

static void error_at_current(const char *msg)
{
	error_at(&parser.current, msg);
}

static void error(const char *msg)
{
	error_at(&parser.previous, msg);
}

static void consume(TokenType type, const char *msg)
{
	if (parser.current.type == type) {
		advance();
		return;
	}

	error_at_current(msg);
}

static bool check(TokenType t)
{
	return parser.current.type == t;
}

static bool match(TokenType t)
{
	if (!check(t))
		return false;

	advance();

	return true;
}

static char *copy_string(const char *chars, size_t len)
{
	char *heap = malloc(len + 1);
	memcpy(heap, chars, len);
	heap[len] = '\0';

	return heap;
}

static char *parse_identifier(const char *err)
{
	consume(TK_IDENTIFIER, err);

	return copy_string(parser.previous.start, parser.previous.length);
}

static void loop(Procedure *p);
static void call(Procedure *p);

static void instruction(Procedure *p)
{
	advance();

	switch (parser.previous.type) {
	case TK_PLUS:
		vm_bytecode_push(&p->bc, OP_INC);
		break;
	case TK_MINUS:
		vm_bytecode_push(&p->bc, OP_DEC);
		break;
	case TK_RIGHT:
		vm_bytecode_push(&p->bc, OP_NEXT);
		break;
	case TK_LEFT:
		vm_bytecode_push(&p->bc, OP_PREV);
		break;
	case TK_DOT:
		vm_bytecode_push(&p->bc, OP_OUT);
		break;
	case TK_COMMA:
		vm_bytecode_push(&p->bc, OP_IN);
		break;
	case TK_LEFT_SQUARE:
		loop(p);
		break;
	case TK_RIGHT_SQUARE:
		error("unmatched ]");
		break;
	case TK_BANG:
		if (p->name != NULL)
			error("nested procedures are not allowed in beshfuck");
		break;
	case TK_RIGHT_BRACE:
		error("unmatched }");
		break;
	case TK_DOLLAR:
		call(p);
		break;
	default:
		break;
	}
}

// TODO: use uint16_t for jump operand
static void loop(Procedure *p)
{
	int loop_start = p->bc.count;

	// emit jump
	vm_bytecode_push(&p->bc, OP_JUMP_IF_ZERO);
	vm_bytecode_push(&p->bc, 0xff);
	int exit_jump = p->bc.count - 1;

	while (!check(TK_RIGHT_SQUARE) && !check(TK_EOF)) {
		if (check(TK_BANG))
			error("procedure declaration inside loop is not allowed");

		instruction(p);
	}

	// emit loop
	vm_bytecode_push(&p->bc, OP_LOOP);
	int offset = p->bc.count - loop_start + 1;
	if (offset > UINT8_MAX)
		error("loop body too large");

	vm_bytecode_push(&p->bc, (uint8_t)offset);

	// patch jump
	int jump = p->bc.count - exit_jump - 1;
	if (jump > UINT8_MAX)
		error("cant jump that far");

	p->bc.code[exit_jump] = (uint8_t)jump;

	consume(TK_RIGHT_SQUARE, "expected ] after loop body");
}

static void procedure(Vm *vm)
{
	char *name = parse_identifier("expected procedure name after !");
	consume(TK_LEFT_BRACE, "expected { after procedure name");

	Procedure *p = vm_procedure_new(name);

	while (!check(TK_RIGHT_BRACE) && !check(TK_EOF)) {
		instruction(p);
	}

	vm_bytecode_push(&p->bc, OP_RETURN);

	consume(TK_RIGHT_BRACE, "expected } after procedure body");

	map_set(vm->procedures, name, p);

	// vm_debug_disassemble(&p->bc, name, NULL);
}

static void call(Procedure *p)
{
	char *name = parse_identifier("expected procedure name after $");
	size_t proc = vm_bytecode_add_constant(&p->bc, name);

	vm_bytecode_push(&p->bc, OP_CALL);
	vm_bytecode_push(&p->bc, proc);
}

Procedure *compiler_do(Vm *vm, const char *source)
{
	assert(source);
	assert(vm);

	lexer_init(source);

	Procedure *main = vm_procedure_new(NULL);

	advance();
	while (!match(TK_EOF)) {
		if (match(TK_BANG))
			procedure(vm);
		else
			instruction(main);
	}

	vm_bytecode_push(&main->bc, OP_RETURN);

	// vm_debug_disassemble(&main->bc, "MAIN", NULL);
	return parser.had_error ? NULL : main;
}
