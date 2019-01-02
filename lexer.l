%{
  #include <stdlib.h>
  #include "ast.h"
  #include "parser.h"
  int yyerror(char *errormsg);
%}

%option nounput
%option noinput

%%

"#pragma"[ ]+"MPC"[ ]+"precision("[0-9]+")"[ ]+"rounding("[.]+")" {}

"int"      { return INT; }
"long"     { return LONG; }
"short"    { return SHORT; }
"float"    { return FLOAT; }
"double"   { return DOUBLE; }
"char"     { return CHAR; }

"for"      { return FOR; }
"while"    { return WHILE; }
"do"       { return DO; }
"switch"   { return SWITCH; }
"case"     { return CASE; }
"default"  { return DEFAULT; }

"break"    { return BREAK; }
"continue" { return CONTINUE; }

"if"       { return IF; }
"else"     { return ELSE; }
"return"   { return RETURN; }

"extern"   { return EXTERN; }


"//"       { return COMMENT_LINE; }
"/*"       { return COMMENT_MULTI; }
"*/"       { return COMMENT_END; }
";"        { return ';'; }
"="        { return '='; }
"+"        { return '+'; }
"*"        { return '*'; }
"-"        { return '-'; }
"/"        { return '/'; }
"("        { return '('; }
")"        { return ')'; }
"{"        { return '{'; }
"}"        { return '}'; }
"."        { return '.'; }

"++"       { yylval.unary = OP_INCR;   return UNARY_OP; }
"--"       { yylval.unary = OP_DECR;   return UNARY_OP; }

"abs"      { yylval.unary = OP_ABS;    return UNARY_FUNC; }
"cpowf"    { yylval.unary = OP_CPOWF;  return UNARY_FUNC; }
"cpowl"    { yylval.unary = OP_CPOWL;  return UNARY_FUNC; }
"cpow"     { yylval.unary = OP_CPOW;   return UNARY_FUNC; }
"csinf"    { yylval.unary = OP_CSINF;  return UNARY_FUNC; }
"csinl"    { yylval.unary = OP_CSINL;  return UNARY_FUNC; }
"csin"     { yylval.unary = OP_CSIN;   return UNARY_FUNC; }
"csqrtf"   { yylval.unary = OP_CSQRTF; return UNARY_FUNC; }
"csqrtl"   { yylval.unary = OP_CSQRTL; return UNARY_FUNC; }
"csqrt"    { yylval.unary = OP_CSQRT;  return UNARY_FUNC; }
"exp"      { yylval.unary = OP_EXP;    return UNARY_FUNC; }
"log"      { yylval.unary = OP_LOG;    return UNARY_FUNC; }
"powf"     { yylval.unary = OP_POWF;   return UNARY_FUNC; }
"powl"     { yylval.unary = OP_POWL;   return UNARY_FUNC; }
"pow"      { yylval.unary = OP_POW;    return UNARY_FUNC; }
"sinf"     { yylval.unary = OP_SINF;   return UNARY_FUNC; }
"sinl"     { yylval.unary = OP_SINL;   return UNARY_FUNC; }
"sin"      { yylval.unary = OP_SIN;    return UNARY_FUNC; }
"sqrtf"    { yylval.unary = OP_SQRTF;  return UNARY_FUNC; }
"sqrtl"    { yylval.unary = OP_SQRTL;  return UNARY_FUNC; }
"sqrt"     { yylval.unary = OP_SQRT;   return UNARY_FUNC; }

[0-9]+          { yylval.integer = atoi(yytext); return INTEGER; }
[0-9]+"."[0-9]+ { yylval.decimal = atof(yytext); return DECIMAL; }

"\n" { return '\n'; }
.    {}

%%

int yywrap(void) {
     return 0;
}

int yyerror(char *errormsg) {
      fprintf(stderr, "%s\n", errormsg);
      exit(1);
}
