#include <assert.h>
#include <stdio.h>
#include <mpfr.h>
#include <math.h>

int main(void)
{
  double x = 5, y, z;
  
  // declaration of all variables that we will use
  mpfr_t c2mp_temp0; mpfr_init2(c2mp_temp0, 128);
  mpfr_t c2mp_temp1; mpfr_init2(c2mp_temp1, 128);
  mpfr_t c2mp_temp2; mpfr_init2(c2mp_temp2, 128);
  mpfr_t c2mp_temp3; mpfr_init2(c2mp_temp3, 128);
  mpfr_t c2mp_temp4; mpfr_init2(c2mp_temp4, 128);
  mpfr_t c2mp_temp5; mpfr_init2(c2mp_temp5, 128);
  mpfr_t c2mp_temp6; mpfr_init2(c2mp_temp6, 128);
  mpfr_t c2mp_temp7; mpfr_init2(c2mp_temp7, 128);
  mpfr_t c2mp_temp8; mpfr_init2(c2mp_temp8, 128);
  mpfr_t c2mp_temp9; mpfr_init2(c2mp_temp9, 128); // x

  // assign values to some variables
  mpfr_set_d(c2mp_temp0, 0.000000, MPFR_RNDZ);
  mpfr_set_d(c2mp_temp3, 2.000000, MPFR_RNDZ);
  mpfr_set_d(c2mp_temp6, 5.000000, MPFR_RNDZ);
  mpfr_set_d(c2mp_temp8, 1.000000, MPFR_RNDZ);
  mpfr_set_d(c2mp_temp9, x, MPFR_RNDZ); // x

  // operations
  z = mpfr_get_d(c2mp_temp9, MPFR_RNDZ);
  y = mpfr_get_d(c2mp_temp9, MPFR_RNDZ);
  x = mpfr_get_d(c2mp_temp9, MPFR_RNDZ);

  // free memory of all variables that we used
  mpfr_clear(c2mp_temp0);
  mpfr_clear(c2mp_temp1);
  mpfr_clear(c2mp_temp2);
  mpfr_clear(c2mp_temp3);
  mpfr_clear(c2mp_temp4);
  mpfr_clear(c2mp_temp5);
  mpfr_clear(c2mp_temp6);
  mpfr_clear(c2mp_temp7);
  mpfr_clear(c2mp_temp8);
  mpfr_clear(c2mp_temp9);


  assert(x == y && y == z);
  return 0;
}
