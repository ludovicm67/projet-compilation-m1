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




%%

int yywrap(void) {
     return 0;
}

int yyerror(char *errormsg) {
      fprintf(stderr, "%s\n", errormsg);
      exit(1);
}
