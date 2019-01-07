#include <complex.h>
#include <stdio.h>
#include <mpc.h>
#include <math.h>

int main() {
  double complex resultat;

  
  // declaration of all variables that we will use
  mpc_t c2mp_temp0; mpc_init2(c2mp_temp0, 256); // resultat
  mpc_t c2mp_temp1; mpc_init2(c2mp_temp1, 256);
  mpc_t c2mp_temp2; mpc_init2(c2mp_temp2, 256);
  mpc_t c2mp_temp3; mpc_init2(c2mp_temp3, 256);

  // assign values to some variables
  mpc_set_d(c2mp_temp2, 3.000000, MPC_RNDZZ);
  mpc_set_d(c2mp_temp3, 3.000000, MPC_RNDZZ);

  // operations
  mpc_mul(c2mp_temp1, c2mp_temp3, c2mp_temp2, MPC_RNDZZ); // c2mp_temp1 = c2mp_temp3 * c2mp_temp2
  mpc_set(c2mp_temp0, c2mp_temp1, MPC_RNDZZ); // c2mp_temp0 = c2mp_temp1
  resultat = mpc_get_dc(c2mp_temp0, MPC_RNDZZ);

  // free memory of all variables that we used
  mpc_clear(c2mp_temp0);
  mpc_clear(c2mp_temp1);
  mpc_clear(c2mp_temp2);
  mpc_clear(c2mp_temp3);


  printf("==> %f%+fi\n", creal(resultat), cimag(resultat));

  return 0;
}
