#include <stdio.h>
#include <mpfr.h>

int main(void)
{
  // This checks that a declaration without value should be declared in the
  // generated code.
  
  // declaration of all variables that we will use
  mpfr_t c2mp_temp0; mpfr_init2(c2mp_temp0, 128); // x

  // assign values to some variables
  double x;

  // free memory of all variables that we used
  mpfr_clear(c2mp_temp0);


  x = 2;

  printf("%f", x);
  return 0;
}
