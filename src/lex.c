#include "include/lex.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>


//Create a new lexer
lexer_T* init_lexer(char* contents)
{
    lexer_T* lexer = calloc(1, sizeof(struct LEXER_STRUCT));
    lexer->contents = contents;
    lexer->i=0;
    lexer->c=contents[lexer->i];
    return lexer;
}

//Advances and returns a token
token_T* lexer_advance_with_token(lexer_T* lexer, token_T* token)
{
    lexer_advance(lexer);
    return token;
}

//Advance - move to next char
void lexer_advance(lexer_T* lexer)
{
    if (lexer->c != '\0' && lexer->i < strlen(lexer->contents)) // checks if c is not equals to null
    {
        lexer->i += 1;
        lexer->c = lexer->contents[lexer->i];
    }
    else
    {
        lexer->c = '\0';
    }
}


//Move to next character until there is no more whitespace
void lexer_skip_whitespace(lexer_T* lexer)
{
    while (lexer->c == ' ' || (int) lexer-> c == 10 || (int) lexer->c == 13) //10 ==  new line
    {
        lexer_advance(lexer);
    }
}

//Skip an inline comment
void lexer_skip_inline_comment(lexer_T* lexer)
{
    while (lexer->c != '\n' && lexer->c != 10)
        lexer_advance(lexer);
}

//Get the next token from the lexer
token_T* lexer_get_next_token(lexer_T* lexer)
{
    while (lexer->c != '\0' && lexer->i < strlen(lexer->contents))
    {
        if (lexer->c == ' ' || (int) lexer-> c == 10 || (int) lexer->c == 13)
            lexer_skip_whitespace(lexer);

        if (isalnum(lexer->c) || lexer->c == '_')
            return lexer_collect_id(lexer);
            
        
        if (lexer->c == '#')
        {
            lexer_advance(lexer);
            lexer_skip_inline_comment(lexer);
            continue;
        }

        switch (lexer->c)
        {
            case '"': return lexer_collect_string(lexer); break;
            case '=': return lexer_advance_with_token(lexer, init_token(TOKEN_EQUALS, lexer_get_current_char_as_string(lexer))); break;
            case ';': return lexer_advance_with_token(lexer, init_token(TOKEN_SEMI, lexer_get_current_char_as_string(lexer))); break;
            case '(': return lexer_advance_with_token(lexer, init_token(TOKEN_LPAREN, lexer_get_current_char_as_string(lexer))); break;
            case ')': return lexer_advance_with_token(lexer, init_token(TOKEN_RPAREN, lexer_get_current_char_as_string(lexer))); break;
            case '{': return lexer_advance_with_token(lexer, init_token(TOKEN_LBRACE, lexer_get_current_char_as_string(lexer))); break;
            case '}': return lexer_advance_with_token(lexer, init_token(TOKEN_RBRACE, lexer_get_current_char_as_string(lexer))); break;
            case ',': return lexer_advance_with_token(lexer, init_token(TOKEN_COMMA, lexer_get_current_char_as_string(lexer))); break;
            //case '+': return lexer_advance_with_token(lexer, init_token(TOKEN_PLUS, lexer_get_current_char_as_string(lexer))); break;
        }
    }
    return init_token(TOKEN_EOF, "\0");
}

//Collects a string token
token_T* lexer_collect_string(lexer_T* lexer)
{
    lexer_advance(lexer);

    char* value = calloc(1, sizeof(char));
    value[0] = '\0';

    while (lexer->c != '"')
    {
        char* s = lexer_get_current_char_as_string(lexer);
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);

        lexer_advance(lexer);
    }

    lexer_advance(lexer);

    return init_token(TOKEN_STRING, value);
}

//Todo: collect numbers

//Collect an Id token
token_T* lexer_collect_id(lexer_T* lexer)
{
    char* value = calloc(1, sizeof(char));
    value[0] = '\0';

    while (isalnum(lexer->c) || lexer->c == '_')
    {
        char* s = lexer_get_current_char_as_string(lexer);
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);

        lexer_advance(lexer);
    }

    return init_token(TOKEN_ID, value);
}

//Return current char of lexer as a string
char* lexer_get_current_char_as_string(lexer_T* lexer)
{
    char* str = calloc(2, sizeof(char));
    str[0] = lexer->c;
    str[1] = '\0';
    return str;
}