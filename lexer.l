%{
  #include <stdlib.h>
  #include <string.h>
  #include "ast.h"
  #include "statement.h"
  #include "parser.h"
  int yyerror(char *errormsg);
%}

%option nounput
%option noinput

%%

"#pragma"             { return PRAGMA; }
" MPC"                { return MPC; }
" MPFR"               { return MPFR; }
" precision"          { return PRECISION; }
" rounding"           { return ROUNDING; }

"int"                  { return INT; }
"float"                { return FLOAT; }
"double"               { return DOUBLE; }
"bool"                 { return BOOL; }

"for"                  { return FOR; }
"while"                { return WHILE; }
"do"                   { return DO; }

"break"                { return BREAK; }
"continue"             { return CONTINUE; }

"if"                   { return IF; }
"else"                 { return ELSE; }
"return"               { return RETURN; }

"extern"               { return EXTERN; }


"//"                   { return COMMENT_LINE; }
"/*"                   { return COMMENT_MULTI; }
"*/"                   { return COMMENT_END; }

";"                    { return ';'; }
"="                    { return '='; }
"+"                    { return '+'; }
"*"                    { return '*'; }
"-"                    { return '-'; }
"/"                    { return '/'; }
"("                    { return '('; }
")"                    { return ')'; }
"{"                    { return '{'; }
"}"                    { return '}'; }
"."                    { return '.'; }
","                    { return ','; }
"!"                    { return '!'; }
">"                    { yylval.binary = OP_GT;    return COMP_OP; }
"<"                    { yylval.binary = OP_LT;    return COMP_OP; }
"<="                   { yylval.binary = OP_LTE;   return COMP_OP; }
">="                   { yylval.binary = OP_GTE;   return COMP_OP; }
"=="                   { yylval.binary = OP_EQ;    return EQ_OP; }
"!="                   { yylval.binary = OP_EQ;    return EQ_OP; }

"&&"                   { return AND_OP; }
"||"                   { return OR_OP; }

"++"                   { yylval.unary = OP_INCR;   return UNARY_OP; }
"--"                   { yylval.unary = OP_DECR;   return UNARY_OP; }

"cabsf"                { yylval.unary = OP_CABSF;  return UNARY_FUNC; }
"cabsl"                { yylval.unary = OP_CABSL;  return UNARY_FUNC; }
"cabs"                 { yylval.unary = OP_CABS;   return UNARY_FUNC; }
"ccosf"                { yylval.unary = OP_CCOSF;  return UNARY_FUNC; }
"ccosl"                { yylval.unary = OP_CCOSL;  return UNARY_FUNC; }
"ccos"                 { yylval.unary = OP_CCOS;   return UNARY_FUNC; }
"cexpf"                { yylval.unary = OP_CEXPF;  return UNARY_FUNC; }
"cexpl"                { yylval.unary = OP_CEXPL;  return UNARY_FUNC; }
"cexp"                 { yylval.unary = OP_CEXP;   return UNARY_FUNC; }
"clogf"                { yylval.unary = OP_CLOGF;  return UNARY_FUNC; }
"clogl"                { yylval.unary = OP_CLOGL;  return UNARY_FUNC; }
"clog"                 { yylval.unary = OP_CLOG;   return UNARY_FUNC; }
"cpow"                 { yylval.unary = OP_CPOW;   return UNARY_FUNC; }
"cpowf"                { yylval.unary = OP_CPOWF;  return UNARY_FUNC; }
"cpowl"                { yylval.unary = OP_CPOWL;  return UNARY_FUNC; }
"csin"                 { yylval.unary = OP_CSIN;   return UNARY_FUNC; }
"csinf"                { yylval.unary = OP_CSINF;  return UNARY_FUNC; }
"csinl"                { yylval.unary = OP_CSINL;  return UNARY_FUNC; }
"csqrt"                { yylval.unary = OP_CSQRT;  return UNARY_FUNC; }
"csqrtf"               { yylval.unary = OP_CSQRTF; return UNARY_FUNC; }
"csqrtl"               { yylval.unary = OP_CSQRTL; return UNARY_FUNC; }
"llabs"                { yylval.unary = OP_LLABS;  return UNARY_FUNC; }
"labs"                 { yylval.unary = OP_LABS;   return UNARY_FUNC; }
"abs"                  { yylval.unary = OP_ABS;    return UNARY_FUNC; }
"cosf"                 { yylval.unary = OP_COSF;   return UNARY_FUNC; }
"cosl"                 { yylval.unary = OP_COSL;   return UNARY_FUNC; }
"cos"                  { yylval.unary = OP_COS;    return UNARY_FUNC; }
"expf"                 { yylval.unary = OP_EXPF;   return UNARY_FUNC; }
"expl"                 { yylval.unary = OP_EXPL;   return UNARY_FUNC; }
"exp"                  { yylval.unary = OP_EXP;    return UNARY_FUNC; }
"logf"                 { yylval.unary = OP_LOGF;   return UNARY_FUNC; }
"logl"                 { yylval.unary = OP_LOGL;   return UNARY_FUNC; }
"log"                  { yylval.unary = OP_LOG;    return UNARY_FUNC; }
"pow"                  { yylval.unary = OP_POW;    return UNARY_FUNC; }
"powf"                 { yylval.unary = OP_POWF;   return UNARY_FUNC; }
"powl"                 { yylval.unary = OP_POWL;   return UNARY_FUNC; }
"sin"                  { yylval.unary = OP_SIN;    return UNARY_FUNC; }
"sinf"                 { yylval.unary = OP_SINF;   return UNARY_FUNC; }
"sinl"                 { yylval.unary = OP_SINL;   return UNARY_FUNC; }
"sqrt"                 { yylval.unary = OP_SQRT;   return UNARY_FUNC; }
"sqrtf"                { yylval.unary = OP_SQRTF;  return UNARY_FUNC; }
"sqrtl"                { yylval.unary = OP_SQRTL;  return UNARY_FUNC; }

[0-9]+                 { yylval.integer = atoi(yytext); return INTEGER; }
[0-9]+"."[0-9]+        { yylval.decimal = atof(yytext); return DECIMAL; }

[\n\t ]                {}

[a-zA-Z_][0-9a-zA-Z_]* { yylval.identifier = strdup(yytext); return IDENTIFIER; }

%%

int yywrap(void) {
     return 0;
}

int yyerror(char *errormsg) {
      fprintf(stderr, "%s\n", errormsg);
      exit(1);
}
