%{
  #include "parser.h"
  int yyerror(char *errormsg);
%}

%option nounput
%option noinput

%%

"#pragma"[ ]+"MPC"[ ]+"precision("[0-9]+")"[ ]+"rounding("[.]+")" {}

"char" {return CHAR;}
"continue" {return CONTINUE;}
"default" {return DEFAULT;}
"do" {return DO;}
"double" {return DOUBLE;}
"else" {return ELSE;}
"extern" {return EXTERN;}
"float" {return FLOAT;}
"for" {return FOR;}
"if" {return IF;}
"int" {return INT;}
"long" {return LONG;}
"return" {return RETURN;}
"short" {return SHORT;}
"switch" {return SWITCH;}
"while" {return WHILE;}


"//" {return COMMENT_LINE;}
"/*" {return COMMENT_MULTI;}
"*/" {return COMMENT_END;}
";"  {return ';';}
"=" {return '=';}
"+" {return '+';}
"*" {return '*';}
"-" {return '-';}
"/" {return '/';}
"(" {return '(';}
")" {return ')';}
"{" {return '{';}
"}" {return '}';}
"." {return '.';}


"pow" { yylval.unary = OP_POW;  return UNARY_FUNC; }
"abs" { yylval.unary = OP_ABS;  return UNARY_FUNC; }
"++"  { yylval.unary = OP_INCR; return UNARY_OP; }
"*"   { yylval.unary = OP_INCR; return UNARY_OP; }

%%

int yywrap(void) {
     return 0;
}

int yyerror(char *errormsg) {
      fprintf(stderr, "%s\n", errormsg);
      exit(1);
}
