#include <stdio.h>

#include "lexer.h"
#include "_parser.h"

#define LINE 255

int main()
{

    char buffer[LINE + 1];
    printf("Ingrese una expresion: ");

    int i;
    int ch;
    for (i = 0; (i < LINE) && ((ch = getchar()) != EOF) && (ch != '\n') ;++i) buffer[i] = ch;
    buffer[i] = '\0'; // final del string

    printf("Su expresion es: %s\n", buffer);

    lexer_t*    lexer  = init_lexer(buffer, sizeof(buffer));
    puts("Lexer finalizado.");
    parser_t*   parser = init_parser(lexer);
    puts("Parser finalizado.");

    lexer_t* lexer_print = init_lexer(buffer, sizeof(buffer));
    print_tokents(lexer_print);
    puts("print_tokents Parser finalizado.");
    free(lexer_print);
    lexer_print = NULL;


    ast_t*      root  = parser_parser(parser);
    puts("ast finalizado.");
    print_ast(root, 0, "");

    int result = calc_value(root);
    printf("El resultado es: %d\n", result);
    root = free_ast(root);
    parser = free_parser(parser);
    free_lexer(lexer);

    return 0;
}