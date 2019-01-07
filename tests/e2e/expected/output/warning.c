#include <stdio.h>
#include <mpfr.h>

int main(void)
{
  double x, y = 2;
  
  // declaration of all variables that we will use
  mpfr_t c2mp_temp0; mpfr_init2(c2mp_temp0, 128); // x
  mpfr_t c2mp_temp1; mpfr_init2(c2mp_temp1, 128);
  mpfr_t c2mp_temp2; mpfr_init2(c2mp_temp2, 128); // x
  mpfr_t c2mp_temp3; mpfr_init2(c2mp_temp3, 128); // y

  // assign values to some variables
  mpfr_set_d(c2mp_temp1, 5.000000, MPFR_RNDZ);
  mpfr_set_d(c2mp_temp3, y, MPFR_RNDZ); // y

  // operations
  mpfr_set(c2mp_temp2, c2mp_temp3, MPFR_RNDZ); // c2mp_temp2 = c2mp_temp3
  mpfr_set(c2mp_temp0, c2mp_temp1, MPFR_RNDZ); // c2mp_temp0 = c2mp_temp1
  x = mpfr_get_d(c2mp_temp0, MPFR_RNDZ);

  // free memory of all variables that we used
  mpfr_clear(c2mp_temp0);
  mpfr_clear(c2mp_temp1);
  mpfr_clear(c2mp_temp2);
  mpfr_clear(c2mp_temp3);


  printf("%f, %f", x, y);
  return 0;
}
