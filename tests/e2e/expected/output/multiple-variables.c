#include <complex.h>
#include <stdio.h>
#include <mpc.h>
#include <math.h>

int main() {
  double complex resultat, s1, s2, s3;

  
  // declaration of all variables that we will use
  mpc_t c2mp_temp0; mpc_init2(c2mp_temp0, 256); // resultat
  mpc_t c2mp_temp1; mpc_init2(c2mp_temp1, 256);
  mpc_t c2mp_temp2; mpc_init2(c2mp_temp2, 256); // s3
  mpc_t c2mp_temp3; mpc_init2(c2mp_temp3, 256);
  mpc_t c2mp_temp4; mpc_init2(c2mp_temp4, 256);
  mpc_t c2mp_temp5; mpc_init2(c2mp_temp5, 256);
  mpc_t c2mp_temp6; mpc_init2(c2mp_temp6, 256); // s2
  mpc_t c2mp_temp7; mpc_init2(c2mp_temp7, 256);
  mpc_t c2mp_temp8; mpc_init2(c2mp_temp8, 256); // s1
  mpc_t c2mp_temp9; mpc_init2(c2mp_temp9, 256);
  mpc_t c2mp_temp10; mpc_init2(c2mp_temp10, 256);

  // assign values to some variables
  mpc_set_d(c2mp_temp4, 3.000000, MPC_RNDZZ);
  mpc_set_d(c2mp_temp5, 4.000000, MPC_RNDZZ);
  mpc_set_d(c2mp_temp10, 9.000000, MPC_RNDZZ);

  // operations
  mpc_sqrt(c2mp_temp9, c2mp_temp10, MPC_RNDZZ); // c2mp_temp9 = sqrt(c2mp_temp10)
  mpc_set(c2mp_temp8, c2mp_temp9, MPC_RNDZZ); // c2mp_temp8 = c2mp_temp9
  mpc_neg(c2mp_temp7, c2mp_temp8, MPC_RNDZZ); // c2mp_temp7 = -c2mp_temp8
  mpc_set(c2mp_temp6, c2mp_temp7, MPC_RNDZZ); // c2mp_temp6 = c2mp_temp7
  mpc_add(c2mp_temp3, c2mp_temp5, c2mp_temp4, MPC_RNDZZ); // c2mp_temp3 = c2mp_temp5 + c2mp_temp4
  mpc_set(c2mp_temp2, c2mp_temp3, MPC_RNDZZ); // c2mp_temp2 = c2mp_temp3
  mpc_mul(c2mp_temp1, c2mp_temp6, c2mp_temp2, MPC_RNDZZ); // c2mp_temp1 = c2mp_temp6 * c2mp_temp2
  mpc_set(c2mp_temp0, c2mp_temp1, MPC_RNDZZ); // c2mp_temp0 = c2mp_temp1
  resultat = mpc_get_dc(c2mp_temp0, MPC_RNDZZ);
  s3 = mpc_get_dc(c2mp_temp2, MPC_RNDZZ);
  s2 = mpc_get_dc(c2mp_temp6, MPC_RNDZZ);
  s1 = mpc_get_dc(c2mp_temp8, MPC_RNDZZ);

  // free memory of all variables that we used
  mpc_clear(c2mp_temp0);
  mpc_clear(c2mp_temp1);
  mpc_clear(c2mp_temp2);
  mpc_clear(c2mp_temp3);
  mpc_clear(c2mp_temp4);
  mpc_clear(c2mp_temp5);
  mpc_clear(c2mp_temp6);
  mpc_clear(c2mp_temp7);
  mpc_clear(c2mp_temp8);
  mpc_clear(c2mp_temp9);
  mpc_clear(c2mp_temp10);


  printf("==> %f%+fi\n", creal(s1), cimag(s1));
  printf("==> %f%+fi\n", creal(s2), cimag(s2));
  printf("==> %f%+fi\n", creal(s3), cimag(s3));
  printf("==> %f%+fi\n", creal(resultat), cimag(resultat));

  return 0;
}
