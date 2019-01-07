#include <stdio.h>
#include <complex.h>
#include <mpc.h>
#include <mpfr.h>
#include <math.h>

int main(void) {
  double x = 2;
  
  // declaration of all variables that we will use
  mpfr_t c2mp_temp0; mpfr_init2(c2mp_temp0, 128);
  mpfr_t c2mp_temp1; mpfr_init2(c2mp_temp1, 128);
  mpfr_t c2mp_temp2; mpfr_init2(c2mp_temp2, 128); // x

  // assign values to some variables
  mpfr_set_d(c2mp_temp1, 2.000000, MPFR_RNDZ);
  mpfr_set_d(c2mp_temp2, x, MPFR_RNDZ); // x

  // operations
  mpfr_mul(c2mp_temp0, c2mp_temp2, c2mp_temp1, MPFR_RNDZ); // c2mp_temp0 = c2mp_temp2 * c2mp_temp1
  x = mpfr_get_d(c2mp_temp0, MPFR_RNDZ);

  // free memory of all variables that we used
  mpfr_clear(c2mp_temp0);
  mpfr_clear(c2mp_temp1);
  mpfr_clear(c2mp_temp2);


  double complex y = 2;
  
  // declaration of all variables that we will use
  mpc_t c2mp_temp3; mpc_init2(c2mp_temp3, 128);
  mpc_t c2mp_temp4; mpc_init2(c2mp_temp4, 128);
  mpc_t c2mp_temp5; mpc_init2(c2mp_temp5, 128); // y

  // assign values to some variables
  mpc_set_d(c2mp_temp4, 2.000000, MPC_RNDZZ);
  mpc_set_d(c2mp_temp5, y, MPC_RNDZZ); // y

  // operations
  mpc_mul(c2mp_temp3, c2mp_temp5, c2mp_temp4, MPC_RNDZZ); // c2mp_temp3 = c2mp_temp5 * c2mp_temp4
  y = mpc_get_dc(c2mp_temp3, MPC_RNDZZ);

  // free memory of all variables that we used
  mpc_clear(c2mp_temp3);
  mpc_clear(c2mp_temp4);
  mpc_clear(c2mp_temp5);


  printf("x: %f, y: %f%+f\n", x, crealf(y), cimagf(y));
}
