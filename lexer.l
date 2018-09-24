%{
  #include "parser.h"
  int yyerror(char *errormsg);
%}

%option nounput
%option noinput

%%

("hi"|"oi")"\n"       { return HI;  }
("tchau"|"bye")"\n"   { return BYE; }
[-[]+.,><]            { return yytext[0]; }
.                     { yyerror("Unknow char");  }

%%

int yywrap(void) {
     return 0;
}

int yyerror(char *errormsg) {
      fprintf(stderr, "%s\n", errormsg);
      exit(1);
}
