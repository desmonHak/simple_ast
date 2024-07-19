#ifndef _TOKENS_H_
#define _TOKENS_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef struct TOKEN_STRING {
    unsigned char* value;
    enum {
        TOKEN_ID,
        TOKEN_EOF,            // para el final del archivo 
        TOKEN_INT,            // para enteros
        TOKEN_suma,           // +
        TOKEN_resta,          // -
        TOKEN_multplicacion,  // *
        TOKEN_division,       // /
        TOKEN_modulo,         // %
    } type;
} token_t;

token_t* init_token(unsigned char* value, unsigned short type);
static const char* token_type_to_str(unsigned int type);

#include "tokens.c"
#endif