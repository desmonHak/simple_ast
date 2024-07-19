#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "ast.h"

typedef struct PARSER_STRUCT
{
    lexer_t *lexer;
    token_t *token;
} parser_t;

ast_t *parser_parser_expr(parser_t *parser);

#include "_parser.c"
#endif