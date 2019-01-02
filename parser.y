%{
  #include <stdio.h>
  #include <stdlib.h>

  int yylex(void);
  int yyerror(const char *s);
%}

%token HI BYE

%union {
  char* identifier;
  char  operator;
  int   value;
}

%token PRAGMA

%token <identifier> ID
%token <operator>   OPERATOR
%token <value>      INTEGER

%token INT LONG SIGNED UNSIGNED FLOAT DOUBLE
%token CHAR SHORT
%token BOOL
%token VOID
%token STRUCT UNION ENUM TYPEDEF

%token FOR WHILE DO
%token IF ELSE SWITCH CASE DEFAULT
%token BREAK CONTINUE GOTO
%token RETURN

%start parse

%%

program:
         hi bye
        ;

hi:
        HI     { printf("Hello World\n");   }
        ;
bye:
        BYE    { printf("Bye World\n"); exit(0); }
         ;
