#ifndef _AST_C_
#define _AST_C_

#include "ast.h"

ast_t* init_ast(ast_t my_ast){
    #ifdef DEBUG_ENABLE
        DEBUG_PRINT(DEBUG_LEVEL_INFO,
                    INIT_TYPE_FUNC_DBG(ast_t*, init_ast)
                        TYPE_DATA_DBG(ast_t , "my_ast = %p")
                            END_TYPE_FUNC_DBG,
                    my_ast);
    #endif
    ast_t* ast = (ast_t*)calloc(1, sizeof(ast_t));
    if (ast) *ast = my_ast;
    return ast;
}

char get_operation_char(type_node n){
    switch (n)
    {
        case AST_ADD: return '+';
        case AST_SUB: return '-';
        case AST_MUL: return '*';
        case AST_DIV: return '/';
        case AST_MOD: return '%';
        default:
            puts("Error get_operation_char, no existe este operador");
            exit(1);
        }
}

int calc_value(ast_t* root) {
    if (root == NULL) {
        puts("Error: el árbol está vacío.");
        return 0;
    }
    int result = 0;

    return result;
}


#endif