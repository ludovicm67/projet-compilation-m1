%{
  #include <stdio.h>
  #include <stdlib.h>

  int yylex(void);
  int yyerror(const char *s);
%}

%union {
  char* identifier;
  char* operator;
  int   value;
  ast_node_t *node;
  ast_unary_op_t unary;
  ast_binary_op_t binary;
}

%token PRAGMA

%token <identifier> IDENTIFIER
%token <operator>   OPERATOR
%token <value>      VALUE

%token INT LONG SHORT SIGNED UNSIGNED FLOAT DOUBLE COMPLEX
%token CHAR
%token BOOL
%token VOID
%token UNARY_FUNC UNARY_OP

%type <unary> UNARY_FUNC
%type <unary> UNARY_OP
%type <node> EXPRESSION

%token FOR WHILE DO
%token IF ELSE SWITCH CASE DEFAULT
%token BREAK CONTINUE GOTO
%token RETURN

%start expression

%%

expression:
    IDENTIFIER
  | VALUE
  | '(' additive_expr ')'
  ;

additive_expr:
    multiplicative_expr
  | additive_expr '+' multiplicative_expr
  | additive_expr '-' multiplicative_expr
  ;

multiplicative_expr:
    expression
  | multiplicative_expr '*' expression
  | multiplicative_expr '/' expression
  ;

unary_expr:
  expression
  | UNARY_OP expression              { $$ = ast_new_unary($1, $2); }
  | UNARY_FUNC '(' additive_expr ')' { $$ = ast_new_unary($1, $3); }
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

