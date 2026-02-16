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
    if (!ast) return NULL;
    
    *ast = my_ast; 
    return ast;
}

void* free_ast(ast_t* ast) {
#ifdef DEBUG_ENABLE
    DEBUG_PRINT(DEBUG_LEVEL_INFO, "Liberando AST en %p", ast);
#endif
    
    if (!ast) return NULL;
    
    // Liberar recursivamente subárboles
    switch (ast->type_node) {
        case AST_ADD:
        case AST_SUB:
        case AST_MUL:
        case AST_DIV:
        case AST_MOD:
            free_ast(ast->data.ast_t_operation.left);
            free_ast(ast->data.ast_t_operation.right);
            break;
        case AST_NUMBER:
        case AST_END:
            // Hojas no tienen hijos
            break;
        default:
            fprintf(stderr, "ERROR: tipo_node desconocido %d\n", ast->type_node);
    }
    
    free(ast);
    return NULL;
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

int calc_value_node(ast_t* node) {
    // ambos subnodos deben ser de tipo numero
    ast_t_operation *vals = &(node->data.ast_t_operation);
    
    // obtener el valor del nodo izquierdo
    int val1 = vals->left->data.ast_t_number.number;

    // obtener el valor del nodo derecho
    int val2 = vals->right->data.ast_t_number.number;

    switch (node->type_node)
    {
        case AST_ADD: return val1 + val2;
        case AST_SUB: return val1 - val2;
        case AST_MUL: return val1 * val2;
        case AST_DIV: return val1 / val2;
        case AST_MOD: return val1 % val2;
    
        default:
            puts("Error calc_value_node, no existe este operador");
            break;
    }
    return 0;
}

int calc_value(ast_t* root) {
    if (root == NULL) {
        puts("Error: el árbol está vacío.");
        return 0;
    }
    int result = 0;

    ast_t* level = root;
    while (level!= NULL) {
        // buscar el ultimo nodo con dos nodos numericos:
        while (level != NULL) {
            switch (level->type_node)
            {
                case AST_ADD:
                case AST_SUB:
                case AST_MUL:
                case AST_DIV:
                case AST_MOD:
                    printf("%p\n", level);

                    // si el nodo derecho es un número
                    if (level->data.ast_t_operation.right->type_node == AST_NUMBER) {
                        goto exit_while;
                    // si el nodo derecho es una operación
                    } else level = level->data.ast_t_operation.right;
                    break;
            default:
                printf("number:  %d\n", level->data.ast_t_number.number);
                break;
            }
        }
        exit_while:
        // si el nivel actual es igual a la raiz, salir del bucle por que se finalizo todo el recorriedo
        if (level == root) break;
        result = calc_value_node(level);
        
        // comvertir el nodo en un nodo numerico
        level->type_node = AST_NUMBER;
        level->data.ast_t_number.number = result; // asinar el resultado de la operacion
        level = root;
        
    }
    // calcular el ultimo nodo(la raiz)
    result = calc_value_node(level);
    return result;
}


#endif