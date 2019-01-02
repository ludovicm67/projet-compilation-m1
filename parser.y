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
}

%token PRAGMA

%token <identifier> IDENTIFIER
%token <operator>   OPERATOR
%token <value>      VALUE

%token INT LONG SHORT SIGNED UNSIGNED FLOAT DOUBLE COMPLEX
%token CHAR
%token BOOL
%token VOID

%token FOR WHILE DO
%token IF ELSE SWITCH CASE DEFAULT
%token BREAK CONTINUE GOTO
%token RETURN

%start expression

%%

expression:
    additive_expr
  | multiplicative_expr
  | unary_expr
  | '(' expression ')'
  ;

unary_expr:
    unary_op IDENTIFIER
  | unary_op VALUE
  | unary_op expression
  | IDENTIFIER
  | VALUE
  ;

additive_expr:
    expression '+' expression
  | expression '-' expression
  ;

multiplicative_expr:
    expression '*' expression
  | expression '/' expression
  ;

unary_op:
    "++"
  | "--"
  | '&'
  | '*'
  | '+'
  | '-'
  | '~'
  | '!'
  | "pow"
  | "powf"
  | "powl"
  | "cpow"
  | "cpowf"
  | "cpowl"
  | "sqrt"
  | "sqrtf"
  | "sqrtl"
  | "csqrt"
  | "csqrtf"
  | "csqrtl"
  | "sin"
  | "sinf"
  | "sinl"
  | "csin"
  | "csinf"
  | "csinl"
  | "log"
  | "exp"
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

