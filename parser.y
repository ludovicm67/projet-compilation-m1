%{
  #include <stdio.h>
  #include <stdlib.h>

  #include "ast.h"
  #include "statement.h"

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
  ast_decl_type_t decl_type;
  stmt_t* stmt;
  struct {
    enum {
      MODE_MPC,
      MODE_MPFR,
    } mode;
    int precision;
    char *rounding;
  } options;
}

%token <identifier> IDENTIFIER
%token <operator>   OPERATOR
%token <integer>    INTEGER
%token <decimal>    DECIMAL

%token PRAGMA MPFR MPC PRECISION ROUNDING

%token INT FLOAT DOUBLE COMPLEX
%token BOOL

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
%type <stmt>  statement
%type <node>  assignement
%type <node>  declaration
%type <stmt>  declaration_list
%type <decl_type> type
%type <integer> precision
%type <identifier> rounding
%type <options> options
%type <options> pragma
%type <stmt> block
%type <stmt> pragma_contents
%type <stmt> statement_list

%start parse

%%

start:
    statement '\n' { stmt_display($1); YYACCEPT; }
  ;

statement:
    additive_expr { $$ = stmt_new($1); }
  | assignement   { $$ = stmt_new($1); }
  | declaration_list
  ;

assignement:
	  IDENTIFIER '=' additive_expr { $$ = ast_new_assign($1, $3); }
  ;

declaration:
    type assignement { $$ = ast_decl_from_assign($1, $2); }
  | type IDENTIFIER  { $$ = ast_new_decl($1, $2, NULL); }
  ;

declaration_list:
    declaration  { $$ = stmt_new($1); }
  | declaration_list ',' IDENTIFIER  { $$ = stmt_push($1, ast_new_decl($1->node->c.decl.type, $3, NULL)); }
  | declaration_list ',' assignement { $$ = stmt_push($1, ast_decl_from_assign($1->node->c.decl.type, $3)); }
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
    IDENTIFIER            { $$ = ast_new_symbol($1); }
  | DECIMAL               { $$ = ast_new_constant($1); }
  | INTEGER               { $$ = ast_new_constant($1); }
  | '(' additive_expr ')' { $$ = $2; }
  ;

type:
    INT     { $$ = TYPE_INT; }
  | FLOAT   { $$ = TYPE_FLOAT; }
  | DOUBLE  { $$ = TYPE_DOUBLE; }
  | COMPLEX { $$ = TYPE_COMPLEX; }
  | BOOL    { $$ = TYPE_BOOL; }
  ;

test:
    pragma '\n' { printf("mode: %d, precision: %d, rounding: %s\n",
                    $1.mode, $1.precision, $1.rounding);
                    YYACCEPT; }
  ;

parse:
    pragma pragma_contents  { printf("mode: %d, precision: %d, rounding: %s\n",
                  $1.mode, $1.precision, $1.rounding);
                  stmt_display($2);
                  YYACCEPT; }
  ;

pragma_contents:
   statement ';'   { $$ = $1; }
 | block           { $$ = $1; }
 ;

statement_list:
  | pragma_contents                { $$ = $1; }
  | statement_list pragma_contents { $$ = $1; stmt_concat(&$$, $2); }
  ;

block:
    '{' '}'                 { $$ = NULL; }
  | '{' statement_list '}'  { $$ = $2; }
  ;

pragma:
    PRAGMA MPC options  { $$.precision = $3.precision;
                          $$.rounding = $3.rounding;
                          $$.mode = MODE_MPC; }
  | PRAGMA MPFR options { $$.precision = $3.precision;
                          $$.rounding = $3.rounding;
                          $$.mode = MODE_MPFR; }
  ;

precision:
    PRECISION '(' INTEGER ')'   { $$ = $3; };

rounding:
    ROUNDING '(' IDENTIFIER ')' { $$ = $3; };

options:
    precision rounding { $$.precision = $1; $$.rounding = $2; }
  | precision          { $$.precision = $1; $$.rounding = NULL; }
  | rounding precision { $$.precision = $2; $$.rounding = $1; }
  | rounding           { $$.precision = 0;  $$.rounding = $1; }
  |                    { $$.precision = 0;  $$.rounding = NULL; }
  ;
