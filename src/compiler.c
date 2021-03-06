#include "compiler.h"

#include "lexer.h"
#include "vm_debug.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	Token current;
	Token previous;
	bool had_error;
	bool panic;
} Parser;

Parser parser;

static void advance()
{
	parser.previous = parser.current;
	parser.current = scan_token();
}

static void error_at(Token *tk, const char *msg)
{
	if (parser.panic)
		return;

	parser.panic = true;

	fprintf(stderr, "[ Syntax error ]");
	if (tk->type == TK_EOF)
		fprintf(stderr, " at end");
	else
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

static void sync()
{
	parser.panic = false;

	while (parser.current.type != TK_EOF) {
		advance();
	}
}

static void loop(Procedure *p);
static void call(Procedure *p);
static void number(Procedure *p);

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
	case TK_COLON:
		vm_bytecode_push(&p->bc, OP_OUT_DEC);
		break;
	case TK_COMMA:
		vm_bytecode_push(&p->bc, OP_IN);
		break;
	case TK_LEFT_SQUARE:
		loop(p);
		break;
	case TK_RIGHT_SQUARE:
		error("unmatched `]`");
		break;
	case TK_BANG:
		if (p->name != NULL)
			error("nested procedures are not allowed in beshfuck");
		break;
	case TK_RIGHT_BRACE:
		error("unmatched `}`");
		break;
	case TK_DOLLAR:
		call(p);
		break;
	case TK_EQUALS:
		number(p);
		break;
	default:
		break;
	}

	if (parser.panic)
		sync();
}

static void loop(Procedure *p)
{
	int loop_start = p->bc.count;

	// emit jump
	vm_bytecode_push(&p->bc, OP_JUMP_IF_ZERO);
	vm_bytecode_push(&p->bc, 0xff);
	vm_bytecode_push(&p->bc, 0xff);
	int32_t exit_jump = p->bc.count - 2;

	while (!check(TK_RIGHT_SQUARE) && !check(TK_EOF)) {
		if (check(TK_BANG))
			error("procedure declaration inside loop is not allowed");

		instruction(p);
	}

	// emit loop
	vm_bytecode_push(&p->bc, OP_LOOP);
	int32_t offset = p->bc.count - loop_start + 2;
	if (offset > UINT16_MAX)
		error("loop body too large");

	vm_bytecode_push(&p->bc, (offset >> 8) & 0xff);
	vm_bytecode_push(&p->bc, offset & 0xff);

	// patch jump
	int jump = p->bc.count - exit_jump - 2;
	if (jump > UINT16_MAX)
		error("cant jump that far");

	p->bc.code[exit_jump] = (jump >> 8) & 0xff;
	p->bc.code[exit_jump + 1] = jump & 0xff;

	consume(TK_RIGHT_SQUARE, "expected `]` after loop body");
}

static void procedure(Vm *vm)
{
	char *name = parse_identifier("expected procedure name after `!`");
	consume(TK_LEFT_BRACE, "expected `{` after procedure name");

	Procedure *p = vm_procedure_new(name);

	while (!check(TK_RIGHT_BRACE) && !check(TK_EOF)) {
		instruction(p);
	}

	vm_bytecode_push(&p->bc, OP_RETURN);

	consume(TK_RIGHT_BRACE, "expected `}` after procedure body");

	map_set(vm->procedures, name, p);

#ifdef DEBUG_PRINT
	vm_debug_disassemble(&p->bc, name);
#endif
}

static void call(Procedure *p)
{
	char *name = parse_identifier("expected procedure name after `$`");
	size_t proc = vm_bytecode_add_constant(&p->bc, name);

	vm_bytecode_push(&p->bc, OP_CALL);
	vm_bytecode_push(&p->bc, proc);
}

static void number(Procedure *p)
{
	consume(TK_NUMBER, "expected number after `=`");
	char *str = copy_string(parser.previous.start, parser.previous.length);
	size_t n = vm_bytecode_add_constant(&p->bc, str);

	vm_bytecode_push(&p->bc, OP_LOAD);
	vm_bytecode_push(&p->bc, (uint8_t)n);
}

Procedure *compiler_do(Vm *vm, const char *source)
{
	assert(source);
	assert(vm);

	lexer_init(source);
	parser.panic = false;
	parser.had_error = false;

	Procedure *main = vm_procedure_new(NULL);

	advance();
	while (!match(TK_EOF)) {
		if (match(TK_BANG))
			procedure(vm);
		else
			instruction(main);
	}

	vm_bytecode_push(&main->bc, OP_RETURN);

#ifdef DEBUG_PRINT
	vm_debug_disassemble(&main->bc, "MAIN");
#endif
	return parser.had_error ? NULL : main;
}
