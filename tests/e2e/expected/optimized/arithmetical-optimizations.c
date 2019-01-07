#include <assert.h>
#include <stdio.h>
#include <mpfr.h>
#include <math.h>

int main(void)
{
  double x = 5, y, z;
  
  // declaration of all variables that we will use
  mpfr_t c2mp_temp24; mpfr_init2(c2mp_temp24, 128); // x

  // assign values to some variables
  mpfr_set_d(c2mp_temp24, x, MPFR_RNDZ); // x

  // operations
  z = mpfr_get_d(c2mp_temp24, MPFR_RNDZ);
  y = mpfr_get_d(c2mp_temp24, MPFR_RNDZ);
  x = mpfr_get_d(c2mp_temp24, MPFR_RNDZ);

  // free memory of all variables that we used
  mpfr_clear(c2mp_temp24);


  assert(x == y && y == z);
  return 0;
}
