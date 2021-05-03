#include "include/visitor.h"
#include "include/scope.h"
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <conio.h>
#endif
#ifdef linux
#include <unistd.h>
#include <termios.h>
#endif

/* Builtin functions */

static AST_T* builtin_function_print(visitor_T* visitor, AST_T** args, int args_size)
{
    for (int i = 0; i < args_size; i++)
    {
        AST_T* visited_ast = visitor_visit(visitor, args[i]);

        switch (visited_ast->type)
        {
            case AST_STRING: printf("%s", visited_ast->string_value); break;
            //other datatypes
            default: break;
        }
    }
    printf("\n");
    return init_ast(AST_NOOP);
}

static AST_T* builtin_function_readline(visitor_T* visitor, AST_T** args, int args_size, AST_T* node)
{
    char input[100];

    if (args_size == 1 || args_size == 2)
    {
        AST_T* visited_ast = visitor_visit(visitor, args[0]);
        switch (visited_ast->type)
        {
            case AST_STRING: printf("%s", visited_ast->string_value); break;
            default: break;
        }
    }
    else{
        printf("Too many arguments given in function \"readline\", Exeptected 2");
        exit(1);
    }


    if (fgets(input, sizeof(input), stdin)) {
        if (args_size == 2)
        {
            //store variable
            //printf("%s", node->variable_name);
        }
    }
    return init_ast(AST_NOOP);
}

static AST_T* builtin_function_waitforkey(visitor_T* visitor, AST_T** args, int args_size)
{
    if (args_size > 1)
    {
        printf("Too many arguments given in function \"waitfkey\", Exeptected 1");
        exit(1);
    }
    for (int i = 0; i < args_size; i++)
    {
        AST_T* visited_ast = visitor_visit(visitor, args[i]);
        switch (visited_ast->type)
        {
            case AST_STRING: printf("%s", visited_ast->string_value); break;
            default: break;
        }
    }

    #ifdef _WIN32
    getch();
    #endif

    #ifdef linux
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if(tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if(tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if(read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if(tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    #endif

    printf("\n");
    return init_ast(AST_NOOP);
}

visitor_T* init_visitor()
{
    visitor_T* visitor = calloc(1, sizeof(struct VISITOR_STRUCT));

    return visitor;
}

AST_T* visitor_visit(visitor_T* visitor, AST_T* node)
{
    switch (node->type)
    {
        case AST_VARIABLE_DEFINITION: return visitor_visit_variable_definition(visitor, node); break;
        case AST_FUNCTION_DEFINITION: return visitor_visit_function_definition(visitor, node); break;
        case AST_VARIABLE: return visitor_visit_variable(visitor, node); break;
        case AST_FUNCTION_CALL: return visitor_visit_function_call(visitor, node); break;
        case AST_STRING: return visitor_visit_string(visitor, node); break;
        case AST_COMPOUND: return visitor_visit_compound(visitor, node); break;
        case AST_NOOP: return node; break;
        default: printf("Uncaught statement of type %d\n", node->type); exit(1); break;
    }
    return init_ast(AST_NOOP); //isnt nessicary
}

AST_T* visitor_visit_variable_definition(visitor_T* visitor, AST_T* node)
{
    scope_add_variable_definition(
        node->scope,
        node
    );
    return node;
}

AST_T* visitor_visit_function_definition(visitor_T* visitor, AST_T* node)
{
    scope_add_function_definition(
        node->scope,
        node
    );
    return node;
}

AST_T* visitor_visit_variable(visitor_T* visitor, AST_T* node)
{
    AST_T* vdef = scope_get_variable_definition(
        node->scope,
        node->variable_name
    );

    if (vdef != (void*)0)
        return visitor_visit(visitor, vdef->variable_definition_value);

    printf("Undefined variable `%s`\n", node->variable_name);
    exit(1);
}

AST_T* visitor_visit_function_call(visitor_T* visitor, AST_T* node)
{
    if (strcmp(node->function_call_name, "say") == 0)
    {
        return builtin_function_print(visitor, node->function_call_arguments, node->function_call_arguments_size);
    }

    if (strcmp(node->function_call_name, "readl") == 0)
    {
        return builtin_function_readline(visitor, node->function_call_arguments, node->function_call_arguments_size, node);
    }

    if (strcmp(node->function_call_name, "waitfkey") == 0)
    {
        return builtin_function_waitforkey(visitor, node->function_call_arguments, node->function_call_arguments_size);
    }

    AST_T* fdef = scope_get_function_definition(
        node->scope,
        node->function_call_name
    ); //fetch function definition

    if(fdef == (void*)0)
    {
        printf("Undefined function `%s`\n", node->function_call_name);
        exit(1);
    }


    if (fdef->function_definition_args_size == node->function_call_arguments_size){
        for (int i = 0; i < fdef->function_definition_args_size; i++) // (int i = 0; i < (int) node->function_call_arguments_size; i++)
        {
            AST_T* ast_var = (AST_T*) fdef->function_definition_args[i];
            AST_T* ast_value = (AST_T*) node->function_call_arguments[i]; //grab called argument values

            AST_T* ast_vardef = init_ast(AST_VARIABLE_DEFINITION); // create variable definition
            ast_vardef->variable_definition_value = ast_value;

            //copy the name from the funciton definition atgument into the new variable definition
            ast_vardef->variable_definition_variable_name = (char*) calloc(strlen(ast_var->variable_name)+1, sizeof(char));
            strcpy(ast_vardef->variable_definition_variable_name, ast_var->variable_name);

            scope_add_variable_definition(fdef->function_definition_body->scope, ast_vardef); //push variable definition into function
        
            //TODO: check if arguments given matches the amount of required by the function
        }
    }
    else if(fdef->function_definition_args_size != node->function_call_arguments_size)
    {
        printf("Function \"%s\" missing \"%d\" required arguments [Line: %d]\n", node->function_call_name, fdef->function_definition_args_size-node->function_call_arguments_size);
        exit(1);
    }

    return visitor_visit(visitor, fdef->function_definition_body);
}

AST_T* visitor_visit_string(visitor_T* visitor, AST_T* node)
{
    return node;
}

AST_T* visitor_visit_compound(visitor_T* visitor, AST_T* node)
{
    for (int i= 0; i < node->compound_size; i++)
    {
        visitor_visit(visitor, node->compound_value[i]);
    }

    return init_ast(AST_NOOP);
}