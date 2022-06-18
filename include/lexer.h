#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

// clang-format off

typedef enum {
        // Simple instructions
	TK_PLUS, TK_MINUS, TK_LEFT, TK_RIGHT, TK_EQUALS,
        TK_DOT, TK_COMMA, TK_RIGHT_SQUARE, TK_LEFT_SQUARE,

        // Literals
        TK_IDENTIFIER, TK_NUMBER,

        // Procedures
        TK_BANG, TK_LEFT_BRACE, TK_RIGHT_BRACE, TK_DOLLAR,

        TK_EOF, TK_SKIP,
} TokenType;

// clang-format on

typedef struct {
	TokenType type;
	const char *start;
	size_t length;
} Token;

void lexer_init(const char *source);
Token scan_token();

#endif
