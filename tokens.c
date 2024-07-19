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

char* token_to_str(token_t* token){
    const char* type_str = token_type_to_str(token->type);
    char template[59];
    if (token->value != NULL && token->value[0] != '\0' && token->value[0] != '\n' && token->value[0] != '\r') 
        strcpy(template, "<type=%s, \tint_type=%d, \tvalue=%s>");
    else 
        strcpy(template, "<type=%s, \tint_type=%d, \tvalue=not printeable>");
    char* str = (char*)calloc(strlen(type_str) + (sizeof(template) / sizeof(const char) + 8), sizeof(char));
    sprintf(str, template, type_str, token->type, token->value);
    return str;

}

#endif