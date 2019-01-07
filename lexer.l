%{
  #include <stdlib.h>
  #include <string.h>
  #include <stdbool.h>

  #include "ast.h"
  #include "statement.h"
  #include "util.h"
  #include "parser.h"

  int yyerror(char *errormsg);
  int scanne(int token);

  bool is_pragma = false;
  bool is_in_single_comment = false;
  bool is_in_multi_comment = false;

  extern FILE *f_dst;
%}

%option noyywrap
%option nounput
%option noinput

%%

<<EOF>>                { return END; }

"#pragma"[ ]+"MPC"     { return PRAGMA_MPC; }
"#pragma"[ ]+"MPFR"    { return PRAGMA_MPFR; }
[ ]+"precision"        { return scanne(PRECISION); }
[ ]+"rounding"         { return scanne(ROUNDING); }

"int"                  { return scanne(INT); }
"float"                { return scanne(FLOAT); }
"double"               { return scanne(DOUBLE); }
"bool"                 { return scanne(BOOL); }

"for"                  { return scanne(FOR); }
"while"                { return scanne(WHILE); }
"do"                   { return scanne(DO); }

"break"                { return scanne(BREAK); }
"continue"             { return scanne(CONTINUE); }

"if"                   { return scanne(IF); }
"else"                 { return scanne(ELSE); }
"return"               { return scanne(RETURN); }

"extern"               { return scanne(EXTERN); }


"//"                   { return scanne(COMMENT_LINE); }
"/*"                   { return scanne(COMMENT_MULTI); }
"*/"                   { if (is_pragma && is_in_multi_comment) return COMMENT_END;
                         return scanne(COMMENT_END); }

">"                    { yylval.binary = OP_GT;    return scanne(COMP_OP); }
"<"                    { yylval.binary = OP_LT;    return scanne(COMP_OP); }
"<="                   { yylval.binary = OP_LTE;   return scanne(COMP_OP); }
">="                   { yylval.binary = OP_GTE;   return scanne(COMP_OP); }
"=="                   { yylval.binary = OP_EQ;    return scanne(EQ_OP); }
"!="                   { yylval.binary = OP_EQ;    return scanne(EQ_OP); }

"&&"                   { return scanne(AND_OP); }
"||"                   { return scanne(OR_OP); }

"++"                   { yylval.unary = OP_INCR;   return scanne(UNARY_OP); }
"--"                   { yylval.unary = OP_DECR;   return scanne(UNARY_OP); }

"cabsf"                { yylval.unary = OP_CABSF;  return scanne(UNARY_FUNC); }
"cabsl"                { yylval.unary = OP_CABSL;  return scanne(UNARY_FUNC); }
"cabs"                 { yylval.unary = OP_CABS;   return scanne(UNARY_FUNC); }
"ccosf"                { yylval.unary = OP_CCOSF;  return scanne(UNARY_FUNC); }
"ccosl"                { yylval.unary = OP_CCOSL;  return scanne(UNARY_FUNC); }
"ccos"                 { yylval.unary = OP_CCOS;   return scanne(UNARY_FUNC); }
"cexpf"                { yylval.unary = OP_CEXPF;  return scanne(UNARY_FUNC); }
"cexpl"                { yylval.unary = OP_CEXPL;  return scanne(UNARY_FUNC); }
"cexp"                 { yylval.unary = OP_CEXP;   return scanne(UNARY_FUNC); }
"clogf"                { yylval.unary = OP_CLOGF;  return scanne(UNARY_FUNC); }
"clogl"                { yylval.unary = OP_CLOGL;  return scanne(UNARY_FUNC); }
"clog"                 { yylval.unary = OP_CLOG;   return scanne(UNARY_FUNC); }
"csin"                 { yylval.unary = OP_CSIN;   return scanne(UNARY_FUNC); }
"csinf"                { yylval.unary = OP_CSINF;  return scanne(UNARY_FUNC); }
"csinl"                { yylval.unary = OP_CSINL;  return scanne(UNARY_FUNC); }
"csqrt"                { yylval.unary = OP_CSQRT;  return scanne(UNARY_FUNC); }
"csqrtf"               { yylval.unary = OP_CSQRTF; return scanne(UNARY_FUNC); }
"csqrtl"               { yylval.unary = OP_CSQRTL; return scanne(UNARY_FUNC); }
"llabs"                { yylval.unary = OP_LLABS;  return scanne(UNARY_FUNC); }
"labs"                 { yylval.unary = OP_LABS;   return scanne(UNARY_FUNC); }
"abs"                  { yylval.unary = OP_ABS;    return scanne(UNARY_FUNC); }
"cosf"                 { yylval.unary = OP_COSF;   return scanne(UNARY_FUNC); }
"cosl"                 { yylval.unary = OP_COSL;   return scanne(UNARY_FUNC); }
"cos"                  { yylval.unary = OP_COS;    return scanne(UNARY_FUNC); }
"expf"                 { yylval.unary = OP_EXPF;   return scanne(UNARY_FUNC); }
"expl"                 { yylval.unary = OP_EXPL;   return scanne(UNARY_FUNC); }
"exp"                  { yylval.unary = OP_EXP;    return scanne(UNARY_FUNC); }
"logf"                 { yylval.unary = OP_LOGF;   return scanne(UNARY_FUNC); }
"logl"                 { yylval.unary = OP_LOGL;   return scanne(UNARY_FUNC); }
"log"                  { yylval.unary = OP_LOG;    return scanne(UNARY_FUNC); }
"sin"                  { yylval.unary = OP_SIN;    return scanne(UNARY_FUNC); }
"sinf"                 { yylval.unary = OP_SINF;   return scanne(UNARY_FUNC); }
"sinl"                 { yylval.unary = OP_SINL;   return scanne(UNARY_FUNC); }
"sqrt"                 { yylval.unary = OP_SQRT;   return scanne(UNARY_FUNC); }
"sqrtf"                { yylval.unary = OP_SQRTF;  return scanne(UNARY_FUNC); }
"sqrtl"                { yylval.unary = OP_SQRTL;  return scanne(UNARY_FUNC); }

"cpow"                 { yylval.binary = OP_CPOW;   return scanne(BINARY_FUNC); }
"cpowf"                { yylval.binary = OP_CPOWF;  return scanne(BINARY_FUNC); }
"cpowl"                { yylval.binary = OP_CPOWL;  return scanne(BINARY_FUNC); }
"pow"                  { yylval.binary = OP_POW;    return scanne(BINARY_FUNC); }
"powf"                 { yylval.binary = OP_POWF;   return scanne(BINARY_FUNC); }
"powl"                 { yylval.binary = OP_POWL;   return scanne(BINARY_FUNC); }

[0-9]+                 { yylval.integer = atoi(yytext); return scanne(INTEGER); }
[0-9]+"."[0-9]+        { yylval.decimal = atof(yytext); return scanne(DECIMAL); }

"\n"                   { if (is_pragma && is_in_single_comment) return yytext[0];
                         if (!is_pragma && f_dst) fprintf(f_dst, "%c", yytext[0]); }
[\t ]+                 { if (!is_pragma && f_dst) fprintf(f_dst, "%s", yytext); }

[a-zA-Z_][0-9a-zA-Z_]* { yylval.identifier = strdup(yytext); return scanne(IDENTIFIER); }

.                      { return scanne(yytext[0]); }

%%

int yyerror(char *errormsg) {
      fprintf(stderr, "%s\n", errormsg);
      exit(1);
}

int scanne(int token) {
  if (is_pragma) {
    if (is_in_single_comment || is_in_multi_comment)
      return IGNORE;
    else
      return token;
  }

  if (f_dst)
    fprintf(f_dst, "%s", yytext);

  return IGNORE;
}
