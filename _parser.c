#ifndef _PARSER_C_
#define _PARSER_C_
#include "_parser.h"

parser_t *init_parser(lexer_t *lexer)
{
    parser_t *parser = (parser_t *)calloc(1, sizeof(parser_t));
    parser->lexer = lexer;
    parser->token = lexer_next_token(lexer);

    return parser;
}

token_t *parser_eat(parser_t *parser, unsigned int type)
{
#ifdef DEBUG_ENABLE
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
                INIT_TYPE_FUNC_DBG(token_t *, parser_eat)
                    TYPE_DATA_DBG(parser_t *, "parser = %p")
                        TYPE_DATA_DBG(unsigned int, "type = %s")
                            END_TYPE_FUNC_DBG,
                parser, token_type_to_str(type));
#endif
    /*
     *
     *  Consume un token, comprueb si es del tipo esperado
     *  y imprime un mensaje de error en el caso de no serlo.
     *  Si es el token esperado avanza el lexerr al siguiente token
     *
     */
    if (parser->token->type != type)
    {
        printf("[Parser]: Token no esperado: %s, se esperaba un: %s \n", token_to_str(parser->token), token_type_to_str(type));
        exit(1);
    }
    parser->token = lexer_next_token(parser->lexer);
    return parser->token;
}

ast_t *parser_parser_operation(parser_t *parser) {
    #ifdef DEBUG_ENABLE
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
                INIT_TYPE_FUNC_DBG(ast_t *, parser_parser_operation)
                    TYPE_DATA_DBG(parser_t *, "parser = %p")
                        END_TYPE_FUNC_DBG,
                parser);
    #endif
    type_node type_N = 0;
    switch (parser->token->type)
    {
        case TOKEN_suma:          type_N = AST_ADD; break;
        case TOKEN_resta:         type_N = AST_SUB; break;
        case TOKEN_multplicacion: type_N = AST_MUL; break;
        case TOKEN_division:      type_N = AST_DIV; break;
        case TOKEN_modulo:        type_N = AST_MOD; break;
            
        default:
            printf("Error parser_parser_operation, se esperaba una operacion arimetica pero se recibido un token %d\n", parser->token->type);
            exit(1);
    }
    return init_ast((ast_t) { .type_node = type_N});
}

ast_t *parser_parser_int(parser_t *parser)
{
#ifdef DEBUG_ENABLE
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
                INIT_TYPE_FUNC_DBG(ast_t *, parser_parser_int)
                    TYPE_DATA_DBG(parser_t *, "parser = %p")
                        END_TYPE_FUNC_DBG,
                parser);
#endif
    /*
     *
     *  Es funcion analiza los valores enteros, creando un nodo AST
     *  de tipo AST_VALUE y colocando dentro el valor analizado en sus hijos
     *
     */
    // obtener el valor antes de eliminarlos

    if (parser->token->value == NULL) {
        puts("parser->token->value es NULL");
        exit(1);
    }
    uint64_t mi_value = atoll(parser->token->value);
    parser_eat(parser, TOKEN_INT);
    ast_t* num = init_ast((ast_t){
        .type_node = AST_NUMBER, 
        .data.ast_t_number.number = mi_value
    });
    // si el siguiente nodo es el final, retornar el numero
    if (parser->token->type == TOKEN_EOF) return num;

    ast_t*nodo_operataion = parser_parser_operation(parser);
    nodo_operataion->data.ast_t_operation.left = num;

    parser->token = lexer_next_token(parser->lexer);
    // crear una nueva rama hacia la derecha
    nodo_operataion->data.ast_t_operation.right = parser_parser_expr(parser);

    return nodo_operataion;
}

ast_t *parser_parser_expr(parser_t *parser)
{
#ifdef DEBUG_ENABLE
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
                INIT_TYPE_FUNC_DBG(ast_t *, parser_parser_expr)
                    TYPE_DATA_DBG(parser_t *, "parser = %p")
                        END_TYPE_FUNC_DBG,
                parser);
#endif
    /*
     *
     *  Esta funcion analiza una expresion y llama a una funcion de analisis.
     *
     */
    printf("Token %s\n", token_to_str(parser->token));
    switch (parser->token->type)
    {
    case TOKEN_INT: return parser_parser_int(parser);
    case TOKEN_EOF:
        puts("Token EOF");
        return init_ast((ast_t){.type_node = AST_END});
    default:
        printf("[Parser]: Se esperaba un token %s\n", token_to_str(parser->token));
        exit(1);
    }
}

ast_t *parser_parser_compound(parser_t *parser)
{
#ifdef DEBUG_ENABLE
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
                INIT_TYPE_FUNC_DBG(ast_t *, parser_parser_compound)
                    TYPE_DATA_DBG(parser_t *, "parser = %p")
                        END_TYPE_FUNC_DBG,
                parser);
#endif
    //ast_t *compound = init_ast((ast_t){.type_node = AST_INIT});
    /*while (parser->token->type != TOKEN_EOF)
    {
        // print_ast_recursive(compound, 0, 1);
        
    }*/
    

    return parser_parser_expr(parser);  ;
}
ast_t *parser_parser(parser_t *parser)
{
#ifdef DEBUG_ENABLE
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
                INIT_TYPE_FUNC_DBG(ast_t *, parser_parser)
                    TYPE_DATA_DBG(parser_t *, "parser = %p")
                        END_TYPE_FUNC_DBG,
                parser);
#endif
    // switch(parser->token->type){}
    return parser_parser_compound(parser);
}

void print_ast(ast_t *my_ast, uint16_t level, char *prefix) {
    if (my_ast != NULL) {
        for (int i = 0; i < level; i++) {
            printf("|  ");
        }
        switch (my_ast->type_node) {
            case AST_ADD:
            case AST_SUB:
            case AST_MUL:
            case AST_DIV:
            case AST_MOD:
                printf("%s%c\n", prefix, get_operation_char(my_ast->type_node));
                print_ast(my_ast->data.ast_t_operation.left, level + 1, "|-- l ->");
                print_ast(my_ast->data.ast_t_operation.right, level + 1, "|-- r ->");
                break;
            case AST_NUMBER:
                printf("%s%d\n", prefix, my_ast->data.ast_t_number);
                break;
            default:
                printf("Error: el valor del nodo AST no existe: %d\n", my_ast->type_node);
                exit(1);
        }
    }
}



#endif