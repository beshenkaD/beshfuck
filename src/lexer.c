#include "lexer.h"
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
	const char *start;
	const char *current;
} Lexer;

Lexer lexer;

void lexer_init(const char *source)
{
	lexer.current = source;
	lexer.start = source;
}

static bool is_at_end()
{
	return *lexer.current == '\0';
}

static Token make_token(TokenType t)
{
	Token tk;
	tk.type = t;
	tk.start = lexer.start;
	tk.length = lexer.current - lexer.start;

	return tk;
}

// static Token error_token(const char *msg)
// {
// 	Token tk;
// 	tk.type = TK_ERROR;
// 	tk.start = msg;
// 	tk.length = strlen(msg);

// 	return tk;
// }

static char advance()
{
	lexer.current++;
	return lexer.current[-1];
}

static char peek()
{
	return *lexer.current;
}

static void skip_whitespaces()
{
	for (;;) {
		char c = peek();

		if (isspace(c))
			advance();
		else
			return;
	}
}

static Token number()
{
	while (isdigit(peek()))
		advance();

	return make_token(TK_NUMBER);
}

static Token identifier()
{
	while (isalpha(peek()) || isdigit(peek()))
		advance();

	return make_token(TK_IDENTIFIER);
}

Token scan_token()
{
	skip_whitespaces();
	lexer.start = lexer.current;

	if (is_at_end())
		return make_token(TK_EOF);

	char c = advance();

	if (isalpha(c))
		return identifier();

	if (isdigit(c))
		return number();

	// clang-format off

	switch (c) {
	case '+': return make_token(TK_PLUS);
	case '-': return make_token(TK_MINUS);
	case '>': return make_token(TK_RIGHT);
	case '<': return make_token(TK_LEFT);
	case '.': return make_token(TK_DOT);
	case ',': return make_token(TK_COMMA);
	case '[': return make_token(TK_LEFT_SQUARE);
	case ']': return make_token(TK_RIGHT_SQUARE);
	case ';': return make_token(TK_SEMICOLON);
	case '!': return make_token(TK_BANG);
	case '{': return make_token(TK_LEFT_BRACE);
	case '}': return make_token(TK_RIGHT_BRACE);
	case '$': return make_token(TK_DOLLAR);
	case '=': return make_token(TK_EQUALS);
	}

	// clang-format on

	return make_token(TK_SKIP);
}
