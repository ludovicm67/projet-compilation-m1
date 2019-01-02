#include "ast.h"
#include "parser.h"
#include <stdio.h>

int main(void) {
  yyparse();

  return 0;
}
