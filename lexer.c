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
void free_lexer(lexer_t* lexer) {
    if (lexer) {
        free(lexer);  // src NO es nuestro, no lo liberamos
    }
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
    
    token_t* tok  = NULL;
    token_t* next = NULL;
    
    tok = lexer_next_token(lexer);
    while (tok->type != TOKEN_EOF) {
        char* data_tok = token_to_str(tok);
        printf("%s\n", data_tok);
        free(data_tok);
        
        next = lexer_next_token(lexer);  // Guarda el siguiente ANTES de liberar
        free_token(tok);               
        tok = next;
    }
    free_token(tok);  // el TOKEN_EOF
    
}

void free_token(token_t* token) {
    if (!token) return;
    
    if (token->value) {   
        free(token->value);
    }
    free(token);
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

/**
 * @brief Es necesario liberar la memoria devuelta
 * 
 * @param lexer 
 * @return token_t* 
 */
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

    // value queda "adoptado" por token
    token_t* tok = init_token(value, TOKEN_INT);  
    if (!tok) {
        free(value);  // Solo si init_token falla
    }
    return tok;
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