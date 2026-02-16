#ifndef _TOKENS_C_
#define _TOKENS_C_
#include "tokens.h"

token_t* init_token(unsigned char* value, unsigned short type){
    // inicializa un token reservando memoria para el y dandole valores
    token_t * my_token = (token_t *)calloc(1, sizeof(token_t));
    my_token->type = type;
    my_token->value = value;
    return my_token;
}

static const char* token_type_to_str(unsigned int type){
    // que sea estativo hace que las cadenas se asignen en la pila
    switch (type)
    {
        case TOKEN_ID: 
            return "TOKEN_ID";
        case TOKEN_EOF:             // para el final del archivo 
            return "TOKEN_EOF";
        case TOKEN_INT:             // para enteros
            return "TOKEN_INT";
        case TOKEN_suma:             
            return "TOKEN_suma";
        case TOKEN_resta:             
            return "TOKEN_resta";
        case TOKEN_multplicacion:             
            return "TOKEN_multplicacion";
        case TOKEN_division:             
            return "TOKEN_division";
        case TOKEN_modulo:             
            return "TOKEN_modulo";
        default:
            printf("Este token no exite"); break;
    }
    return "not_stringable";
}

/**
 * @brief Esta funcion devuelve una cadena reservada dinamicamente con la
 * informacion del token, es importante liberar la memoria despues de usarla
 * 
 * @param token token analizado
 * @return char* puntero a la cadena con la informacion del token, 
 * es importante liberar la memoria despues de usarla
 */
char* token_to_str(const token_t* token) {
    if (!token) return NULL;

    const char* type_str = token_type_to_str(token->type);

    const char* fmt_with_val  = "<type=%s, \tint_type=%d, \tvalue=%s>";
    const char* fmt_no_val    = "<type=%s, \tint_type=%d, \tvalue=not printable>";

    const char* fmt;
    const char* value;

    if (token->value != NULL &&
        token->value[0] != '\0' &&
        token->value[0] != '\n' &&
        token->value[0] != '\r') {
        fmt   = fmt_with_val;
        value = token->value;
    } else {
        fmt   = fmt_no_val;
        value = "";
    }

    int needed = snprintf(NULL, 0, fmt, type_str, token->type, value);
    if (needed < 0) return NULL;

    char* str = malloc((size_t)needed + 1);
    if (!str) return NULL;

    snprintf(str, (size_t)needed + 1, fmt, type_str, token->type, value);
    return str;
}


#endif