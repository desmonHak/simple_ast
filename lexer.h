#ifndef _LEXER_H_
#define _LEXER_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "debug_c.h"
#include "tokens.h"

typedef size_t size_data;

typedef struct LEXER_STRUCT {
    unsigned char *src;  // puntero a los datos
    size_data size;      // tamaño de los datos
    unsigned char c;     // caracter actual
    unsigned int i;      // contador
} lexer_t;

lexer_t* init_lexer(unsigned char *src, size_data size);
void lexer_advance(lexer_t* lexer);
void print_tokents(lexer_t* lexer);
void lexer_skip_whitespace(lexer_t* lexer);
token_t* lexer_parser_number(lexer_t* lexer);
token_t* lexer_next_token(lexer_t* lexer);

#include "lexer.c"
#endif