#ifndef _AST_H_
#define _AST_H_

#include <stdint.h>

typedef struct ast_t ast_t;

typedef struct ast_t_number    { uint64_t number; }           ast_t_number;
typedef struct ast_t_operation { ast_t *left; ast_t *right; } ast_t_operation;

typedef enum type_node
{
    AST_INIT,
    AST_NUMBER,
    AST_ADD,
    AST_SUB,
    AST_MUL,
    AST_DIV,
    AST_MOD,
    AST_END
} type_node;

typedef struct ast_t
{
    type_node type_node;
    union
    {
        ast_t_number    ast_t_number;
        ast_t_operation ast_t_operation;
    } data;
} ast_t;

ast_t* init_ast(ast_t my_ast);
static char get_operation_char(type_node n);

#include "ast.c"
#endif