#ifndef _LEXER_C_
#define _LEXER_C_
#include "lexer.h"

lexer_t* init_lexer(unsigned char *src, size_data size){
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
                    INIT_TYPE_FUNC_DBG(lexer_t *, init_lexer)
                        TYPE_DATA_DBG(unsigned char *src, "lexer = %s")
                        TYPE_DATA_DBG(size_data, "size = %zu")
                        END_TYPE_FUNC_DBG,
                    src,size);
    #endif
    lexer_t *my_lexer = (lexer_t *)calloc(1, sizeof(lexer_t));
    my_lexer->src  = src;                     // puntero a los datos
    my_lexer->size = size;                    // tamaño de los datos
    my_lexer->i = 0;                          // contador
    my_lexer->c = my_lexer->src[my_lexer->i]; // caracter actual
    return my_lexer;
}

void lexer_advance(lexer_t* lexer){
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
                    INIT_TYPE_FUNC_DBG(void, lexer_advance)
                        TYPE_DATA_DBG(lexer_t *, "lexer = %p")
                        END_TYPE_FUNC_DBG,
                    lexer);
    #endif
    if (lexer->i < lexer->size && lexer->c != '\0'){
        lexer->i++;
        lexer->c = lexer->src[lexer->i];
    }
}

void print_tokents(lexer_t* lexer){
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
                    INIT_TYPE_FUNC_DBG(void, print_tokents)
                        TYPE_DATA_DBG(lexer_t *, "lexer = %p")
                        END_TYPE_FUNC_DBG,
                    lexer);
    #endif
    
    token_t* tok;
    for(tok = lexer_next_token(lexer); tok->type != TOKEN_EOF; tok = lexer_next_token(lexer)){
        printf("%s\n", token_to_str(tok));
    }
    
}


void lexer_skip_whitespace(lexer_t* lexer) {
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
                    INIT_TYPE_FUNC_DBG(void, lexer_skip_whitespace)
                        TYPE_DATA_DBG(lexer_t *, "lexer = %p")
                        END_TYPE_FUNC_DBG,
                    lexer);
    #endif
    // si la posicion antual es alguno de estos caracteres " \t\r\n"
    // saltar al siguiente caracter
    while (lexer->c == '\r' || lexer->c == '\t') lexer_advance(lexer);
}

token_t* lexer_parser_number(lexer_t* lexer){
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
                    INIT_TYPE_FUNC_DBG(token_t *, lexer_parser_number)
                        TYPE_DATA_DBG(lexer_t *, "lexer = %p")
                        END_TYPE_FUNC_DBG,
                    lexer);
    #endif
    /*
     *
     *  Esta funcion convierte un valor en entero asignando un token de tipo int
     *  permite detectar si es un valor decimal
     *
     */

    unsigned char * value = (unsigned char*)calloc(1, sizeof(unsigned char));

    while (isdigit(lexer->c))
    {
        value = (unsigned char*)realloc(value, (strlen(value) + 2) * sizeof(unsigned char));
        strcat(value, (char[]){lexer->c, 0});
        lexer_advance(lexer);
    }
    return init_token(value, TOKEN_INT);
}


token_t* lexer_next_token(lexer_t* lexer){
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
                    INIT_TYPE_FUNC_DBG(token_t *, lexer_next_token)
                        TYPE_DATA_DBG(lexer_t *, "lexer = %p")
                        END_TYPE_FUNC_DBG,
                    lexer);
    #endif
    volver:
    while (lexer->c != '\0')
    {
        lexer_skip_whitespace(lexer);
        if (isdigit(lexer->c)) return lexer_parser_number(lexer);
        switch (lexer->c)
        {
            // buscar operadores:
            case '+' : 
                lexer_advance(lexer);
                return init_token(NULL, TOKEN_suma);
            case '-' : 
                lexer_advance(lexer);
                return init_token(NULL, TOKEN_resta);
            case '*' : 
                lexer_advance(lexer);
                return init_token(NULL, TOKEN_multplicacion);
            case '/' : 
                lexer_advance(lexer);
                return init_token(NULL, TOKEN_division);
            case '%' : 
                lexer_advance(lexer);
                return init_token(NULL, TOKEN_modulo);
            case ' ': 
                lexer->c = lexer->src[++lexer->i];
                goto volver;
            case '\0': break;

            default: printf("No se esperaba este caracter: (%hhu), (%c)\n", lexer->c, lexer->c); exit(1); break;
        }
    }

    // si el while no se ejcuto, se encontro el final del archivo:
    return init_token(0, TOKEN_EOF);
}


#endif