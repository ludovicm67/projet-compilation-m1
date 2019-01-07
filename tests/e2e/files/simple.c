#include <stdio.h>
#include <mpfr.h>

int main(void)
{
  // Just a simple test with a signle assignement
  #pragma MPFR rounding(MPFR_RNDZ) precision(128)
  double x = 2;

  printf("%f", x);
  return 0;
}
