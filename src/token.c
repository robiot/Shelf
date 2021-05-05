#include "include/token.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

token_T* init_token(int type, char* value)
{
    token_T* token = calloc(1, sizeof(struct TOKEN_STRUCT));
    token->type = type;
    token->value = value;

    return token;
}

const char* token_type_to_str(int type)
{
  switch (type)
  {
    case TOKEN_ID: return "TOKEN_ID";
    case TOKEN_INT: return "TOKEN_INT";
    case TOKEN_STRING: return "TOKEN_STRING";
    case TOKEN_SEMI: return "TOKEN_SEMI";
    case TOKEN_LPAREN: return "TOKEN_LPAREN";
    case TOKEN_RPAREN: return "TOKEN_RPAREN";
    case TOKEN_LBRACE: return "TOKEN_LBRACE";
    case TOKEN_RBRACE: return "TOKEN_RBRACE";
    case TOKEN_COMMA: return "TOKEN_COMMA";
    case TOKEN_PLUS: return "TOKEN_PLUS";
    case TOKEN_EOF: return "TOKEN_EOF";
  }

  return "not_stringable";
}

char* token_to_str(token_T* token)
{
  const char* type_str = token_type_to_str(token->type);
  const char* template = "<type=`%s`, int_type=`%d`, value=`%s`>";

  char* str = calloc(strlen(type_str) + strlen(template) + 8, sizeof(char));
  sprintf(str, template, type_str, token->type, token->value);

  return str; 
}