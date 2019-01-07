#include <stdio.h>
#include <mpfr.h>

int main(void)
{
  // This checks that a declaration without value should be declared in the
  // generated code.
  #pragma MPFR rounding(MPFR_RNDZ) precision(128)
  double x;

  x = 2;

  printf("%f", x);
  return 0;
}
