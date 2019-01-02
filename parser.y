%{
  #include <stdio.h>
  #include <stdlib.h>

  #include "ast.h"

  int yydebug = 1;
  int yylex(void);
  int yyerror(const char *s);
%}

%union {
  char*  identifier;
  char*  operator;
  int    integer;
  double decimal;
  ast_node_t     *node;
  ast_unary_op_t  unary;
  ast_binary_op_t binary;
}

%token PRAGMA

%token <identifier> IDENTIFIER
%token <operator>   OPERATOR
%token <integer>    INTEGER
%token <decimal>    DECIMAL

%token INT LONG SHORT SIGNED UNSIGNED FLOAT DOUBLE COMPLEX
%token CHAR
%token BOOL
%token VOID

%token UNARY_FUNC UNARY_OP
%token COMMENT_LINE COMMENT_MULTI COMMENT_END
%token EXTERN

%token FOR WHILE DO
%token IF ELSE SWITCH CASE DEFAULT
%token BREAK CONTINUE GOTO
%token RETURN

%type <unary> UNARY_FUNC
%type <unary> UNARY_OP
%type <node>  expression
%type <node>  unary_expr
%type <node>  additive_expr
%type <node>  multiplicative_expr
%type <node>  start

%start start

%%

start:
  additive_expr '\n' { ast_display($1); YYACCEPT; }
  ;

additive_expr:
    multiplicative_expr
  | additive_expr '+' multiplicative_expr { $$ = ast_new_binary(OP_ADD, $1, $3); }
  | additive_expr '-' multiplicative_expr { $$ = ast_new_binary(OP_SUB, $1, $3); }
  ;

multiplicative_expr:
    unary_expr
  | multiplicative_expr '*' unary_expr { $$ = ast_new_binary(OP_MUL, $1, $3); }
  | multiplicative_expr '/' unary_expr { $$ = ast_new_binary(OP_DIV, $1, $3); }
  ;

unary_expr:
    expression
  | '-' expression                   { $$ = ast_new_unary(OP_NEG, $2); }
  | UNARY_OP expression              { $$ = ast_new_unary($1, $2); }
  | UNARY_FUNC '(' additive_expr ')' { $$ = ast_new_unary($1, $3); }
  ;

expression:
    IDENTIFIER  { $$ = ast_new_symbol($1); }
  | DECIMAL     { $$ = ast_new_constant($1); }
  | INTEGER     { $$ = ast_new_constant($1); }
  | '(' additive_expr ')' { $$ = $2; }
  ;

type:
    INT
  | LONG
  | SHORT
  | SIGNED
  | UNSIGNED
  | FLOAT
  | DOUBLE
  | COMPLEX
  | CHAR
  | BOOL
  | VOID
  ;

