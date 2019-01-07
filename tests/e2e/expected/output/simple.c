#include <stdio.h>
#include <mpfr.h>

int main(void)
{
  
  // declaration of all variables that we will use
  mpfr_t c2mp_temp0; mpfr_init2(c2mp_temp0, 128);
  mpfr_t c2mp_temp1; mpfr_init2(c2mp_temp1, 128); // x

  // assign values to some variables
  mpfr_set_d(c2mp_temp0, 2.000000, MPFR_RNDZ);

  // operations
  mpfr_set(c2mp_temp1, c2mp_temp0, MPFR_RNDZ); // c2mp_temp1 = c2mp_temp0
  double x = mpfr_get_d(c2mp_temp1, MPFR_RNDZ);

  // free memory of all variables that we used
  mpfr_clear(c2mp_temp0);
  mpfr_clear(c2mp_temp1);


  printf("%f", x);
  return 0;
}
