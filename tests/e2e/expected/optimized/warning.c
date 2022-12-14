#include <stdio.h>
#include <mpfr.h>

int main(void)
{
  double x, y = 2;
  
  // declaration of all variables that we will use
  mpfr_t c2mp_temp1; mpfr_init2(c2mp_temp1, 128);

  // assign values to some variables
  mpfr_set_d(c2mp_temp1, 5.000000, MPFR_RNDZ);

  // operations
  x = mpfr_get_d(c2mp_temp1, MPFR_RNDZ);

  // free memory of all variables that we used
  mpfr_clear(c2mp_temp1);


  printf("%f, %f", x, y);
  return 0;
}
