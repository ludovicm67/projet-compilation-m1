#include <stdio.h>
#include <mpfr.h>

int main(void)
{
  
  // declaration of all variables that we will use
  mpfr_t c2mp_temp0; mpfr_init2(c2mp_temp0, 128);

  // assign values to some variables
  mpfr_set_d(c2mp_temp0, 2.000000, MPFR_RNDZ);

  // operations
  double x = mpfr_get_d(c2mp_temp0, MPFR_RNDZ);

  // free memory of all variables that we used
  mpfr_clear(c2mp_temp0);


  printf("%f", x);
  return 0;
}
