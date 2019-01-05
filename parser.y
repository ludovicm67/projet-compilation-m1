%{
  #include <stdio.h>
  #include <stdlib.h>

  #include "ast.h"
  #include "statement.h"
  #include "gencode.h"

  int yydebug = 1;
  int yylex(void);
  int yyerror(const char *s);

  extern bool is_pragma;
  extern bool is_in_single_comment;
  extern bool is_in_multi_comment;
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

%token END

%token <identifier> IDENTIFIER
%token <operator>   OPERATOR
%token <integer>    INTEGER
%token <decimal>    DECIMAL

%token IGNORE
%token PRAGMA_MPC PRAGMA_MPFR PRECISION ROUNDING

%token INT FLOAT DOUBLE COMPLEX
%token BOOL

%token AND_OP OR_OP COMP_OP EQ_OP
%token UNARY_FUNC UNARY_OP
%token COMMENT_LINE COMMENT_MULTI COMMENT_END
%token EXTERN

%token FOR WHILE DO
%token IF ELSE
%token BREAK CONTINUE GOTO
%token RETURN

%type <unary>      UNARY_FUNC
%type <unary>      UNARY_OP

%type <binary>     AND_OP
%type <binary>     OR_OP
%type <binary>     COMP_OP
%type <binary>     EQ_OP

%type <node>       expression
%type <node>       unary_expr
%type <node>       additive_expr
%type <node>       multiplicative_expr
%type <node>       comparaison_expr
%type <node>       equal_expr
%type <node>       and_expr
%type <node>       or_expr
%type <node>       assignement_expr
%type <node>       assignement
%type <node>       declaration
%type <stmt>       statement
%type <stmt>       declaration_list
%type <decl_type>  type
%type <integer>    precision
%type <identifier> rounding
%type <options>    options
%type <options>    pragma
%type <stmt>       pragma_contents
%type <stmt>       block
%type <stmt>       statement_list
%type <stmt>       if_statement
%type <stmt>       while_statement
%type <stmt>       do_while_statement
%type <stmt>       for_statement
%type <node>       for_condition
%type <stmt>       for_instruction


%start start

%%

statement:
    assignement_expr ';'  { $$ = stmt_new_expr($1); }
  | declaration_list ';'
  | if_statement
  | while_statement
  | do_while_statement    { $$ = NULL; printf("do_while_statement\n"); }
  | for_statement
  | ';'                   { $$ = NULL; }
  | BREAK ';'             { $$ = NULL; }
  | CONTINUE ';'          { $$ = NULL; }
  ;

declaration:
    type assignement { $$ = ast_decl_from_assign($1, $2); }
  | type IDENTIFIER  { $$ = ast_new_decl($1, $2, NULL); }
  ;

declaration_list:
    declaration                      { $$ = stmt_new_expr($1); }
  | declaration_list ',' assignement { $$ = $1; stmt_concat(&$$, stmt_new_expr(ast_decl_from_assign($1->c.expr->c.decl.type, $3))); }
  | declaration_list ',' IDENTIFIER  { $$ = $1; stmt_concat(&$$, stmt_new_expr(ast_new_decl($1->c.expr->c.decl.type, $3, NULL))); }
  ;

assignement:
	  IDENTIFIER '=' or_expr { $$ = ast_new_assign($1, $3); }
  ;

assignement_expr:
    or_expr
	| assignement
  ;

or_expr:
    and_expr
  | and_expr OR_OP or_expr { $$ = ast_new_binary(OP_OR, $1, $3); }
  ;

and_expr:
    equal_expr
  | equal_expr AND_OP and_expr { $$ = ast_new_binary(OP_AND, $1, $3); }
  ;

equal_expr:
    comparaison_expr
  | comparaison_expr EQ_OP equal_expr { $$ = ast_new_binary($2, $1, $3);  }
  ;

comparaison_expr:
    additive_expr
  | additive_expr COMP_OP comparaison_expr { $$ = ast_new_binary($2, $1, $3); }
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
  | expression UNARY_OP              { $$ = ast_new_unary($2, $1); }
  | UNARY_OP expression              { $$ = ast_new_unary($1, $2); }
  | UNARY_FUNC '(' additive_expr ')' { $$ = ast_new_unary($1, $3); }
  ;

expression:
    IDENTIFIER            { $$ = ast_new_symbol($1); }
  | DECIMAL               { $$ = ast_new_constant($1); }
  | INTEGER               { $$ = ast_new_constant($1); }
  | '(' assignement_expr ')' { $$ = $2; }
  ;

type:
    INT     { $$ = TYPE_INT; }
  | FLOAT   { $$ = TYPE_FLOAT; }
  | DOUBLE  { $$ = TYPE_DOUBLE; }
  | COMPLEX { $$ = TYPE_COMPLEX; }
  | BOOL    { $$ = TYPE_BOOL; }
  ;

start:
    parse_list END { printf("\n"); YYACCEPT; }

parse_list:
    parse
  | parse_list parse
  | parse_list IGNORE
  | IGNORE
  ;

parse:
    pragma pragma_contents  {
      // init gencode args
      gencode_args_t args;
      args.file = stdout;
      args.lib = $1.mode;
      args.precision = $1.precision;
      args.rounding = $1.rounding;

      fprintf(stderr, "mode: %d, precision: %d, rounding: %s\n",
              $1.mode, $1.precision, $1.rounding);
      stmt_display($2);

      op_list_t *ops = NULL;
      symbol_t *table = NULL;
      stmt_gen_quad($2, &table, &ops);

      gencode_init(&args, table);
      gencode_assign(&args, table);
      gencode_operations(&args, ops);
      gencode_clear(&args, table);
      is_pragma = false;
      is_in_single_comment = false;
      is_in_multi_comment = false;
    };

pragma_contents:
    block  { $$ = $1; }
  ;

block:
   statement                { $$ = $1; }
  | '{' '}'                 { $$ = stmt_new_block(NULL); }
  | '{' statement_list '}'  { $$ = stmt_new_block($2); }
  | comment_multiline       { $$ = NULL; }
  | comment_single          { $$ = NULL; }
  ;

statement_list:
    block                { $$ = $1; }
  | statement_list block { $$ = $1; stmt_concat(&$$, $2); }
  ;

pragma:
    PRAGMA_MPC { is_pragma = true; }
               options  { $$.precision = $3.precision;
                          $$.rounding = $3.rounding;
                          $$.mode = MODE_MPC; }
  | PRAGMA_MPFR { is_pragma = true; }
                options { $$.precision = $3.precision;
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

if_statement:
    IF '(' assignement_expr ')' block             { $$ = stmt_new_cond($3, $5, NULL); }
  | IF '(' assignement_expr ')' block ELSE block  { $$ = stmt_new_cond($3, $5, $7); }
  ;

while_statement:
    WHILE '(' assignement_expr ')' block { $$ = stmt_new_loop(NULL, $3, NULL, $5); }
  ;

do_while_statement:
    DO block WHILE ';'  { printf("do while\n"); }
  ;

for_condition:
    assignement_expr
  |                   { $$ = NULL; }
  ;

for_instruction:
    assignement_expr                      { $$ = stmt_new_expr($1); }
  | for_instruction ',' assignement_expr  { $$ = $1; stmt_concat(&$$, stmt_new_expr($3)); }
  |                                       { $$ = NULL; }
  ;

for_statement:
    FOR '(' for_instruction ';' for_condition ';' for_instruction ')' block
      { $$ = stmt_new_loop($3, $5, $7, $9); }
  | FOR '(' declaration_list ';' for_condition ';' for_instruction ')' block
      { $$ = stmt_new_loop($3, $5, $7, $9); }
  ;

comment_multiline:
    COMMENT_MULTI                 { is_in_multi_comment = true; }
  | comment_multiline IGNORE
  | comment_multiline '\n'
  | comment_multiline COMMENT_END { is_in_multi_comment = false; }
  ;

comment_single:
    COMMENT_LINE          { is_in_single_comment = true; }
  | comment_single IGNORE
  | comment_single '\n'   { is_in_single_comment = false; }
  ;
