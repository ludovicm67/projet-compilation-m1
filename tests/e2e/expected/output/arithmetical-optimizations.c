#include <assert.h>
#include <stdio.h>
#include <mpfr.h>
#include <math.h>

int main(void)
{
  double x = 5, y, z;
  
  // declaration of all variables that we will use
  mpfr_t c2mp_temp0; mpfr_init2(c2mp_temp0, 128); // z
  mpfr_t c2mp_temp1; mpfr_init2(c2mp_temp1, 128);
  mpfr_t c2mp_temp2; mpfr_init2(c2mp_temp2, 128);
  mpfr_t c2mp_temp3; mpfr_init2(c2mp_temp3, 128); // y
  mpfr_t c2mp_temp4; mpfr_init2(c2mp_temp4, 128); // y
  mpfr_t c2mp_temp5; mpfr_init2(c2mp_temp5, 128);
  mpfr_t c2mp_temp6; mpfr_init2(c2mp_temp6, 128);
  mpfr_t c2mp_temp7; mpfr_init2(c2mp_temp7, 128);
  mpfr_t c2mp_temp8; mpfr_init2(c2mp_temp8, 128);
  mpfr_t c2mp_temp9; mpfr_init2(c2mp_temp9, 128);
  mpfr_t c2mp_temp10; mpfr_init2(c2mp_temp10, 128);
  mpfr_t c2mp_temp11; mpfr_init2(c2mp_temp11, 128);
  mpfr_t c2mp_temp12; mpfr_init2(c2mp_temp12, 128); // x
  mpfr_t c2mp_temp13; mpfr_init2(c2mp_temp13, 128);
  mpfr_t c2mp_temp14; mpfr_init2(c2mp_temp14, 128);
  mpfr_t c2mp_temp15; mpfr_init2(c2mp_temp15, 128); // x
  mpfr_t c2mp_temp16; mpfr_init2(c2mp_temp16, 128);
  mpfr_t c2mp_temp17; mpfr_init2(c2mp_temp17, 128);
  mpfr_t c2mp_temp18; mpfr_init2(c2mp_temp18, 128); // x
  mpfr_t c2mp_temp19; mpfr_init2(c2mp_temp19, 128);
  mpfr_t c2mp_temp20; mpfr_init2(c2mp_temp20, 128);
  mpfr_t c2mp_temp21; mpfr_init2(c2mp_temp21, 128); // x
  mpfr_t c2mp_temp22; mpfr_init2(c2mp_temp22, 128);
  mpfr_t c2mp_temp23; mpfr_init2(c2mp_temp23, 128);
  mpfr_t c2mp_temp24; mpfr_init2(c2mp_temp24, 128); // x

  // assign values to some variables
  mpfr_set_d(c2mp_temp2, 0.000000, MPFR_RNDZ);
  mpfr_set_d(c2mp_temp7, 2.000000, MPFR_RNDZ);
  mpfr_set_d(c2mp_temp10, 5.000000, MPFR_RNDZ);
  mpfr_set_d(c2mp_temp14, 0.000000, MPFR_RNDZ);
  mpfr_set_d(c2mp_temp17, 1.000000, MPFR_RNDZ);
  mpfr_set_d(c2mp_temp20, 0.000000, MPFR_RNDZ);
  mpfr_set_d(c2mp_temp23, 1.000000, MPFR_RNDZ);
  mpfr_set_d(c2mp_temp24, x, MPFR_RNDZ); // x

  // operations
  mpfr_mul(c2mp_temp22, c2mp_temp24, c2mp_temp23, MPFR_RNDZ); // c2mp_temp22 = c2mp_temp24 * c2mp_temp23
  mpfr_set(c2mp_temp21, c2mp_temp22, MPFR_RNDZ); // c2mp_temp21 = c2mp_temp22
  mpfr_add(c2mp_temp19, c2mp_temp21, c2mp_temp20, MPFR_RNDZ); // c2mp_temp19 = c2mp_temp21 + c2mp_temp20
  mpfr_set(c2mp_temp18, c2mp_temp19, MPFR_RNDZ); // c2mp_temp18 = c2mp_temp19
  mpfr_div(c2mp_temp16, c2mp_temp18, c2mp_temp17, MPFR_RNDZ); // c2mp_temp16 = c2mp_temp18 / c2mp_temp17
  mpfr_set(c2mp_temp15, c2mp_temp16, MPFR_RNDZ); // c2mp_temp15 = c2mp_temp16
  mpfr_sub(c2mp_temp13, c2mp_temp15, c2mp_temp14, MPFR_RNDZ); // c2mp_temp13 = c2mp_temp15 - c2mp_temp14
  mpfr_set(c2mp_temp12, c2mp_temp13, MPFR_RNDZ); // c2mp_temp12 = c2mp_temp13
  mpfr_sin(c2mp_temp11, c2mp_temp12, MPFR_RNDZ); // c2mp_temp11 = sin(c2mp_temp12)
  mpfr_sub(c2mp_temp9, c2mp_temp12, c2mp_temp10, MPFR_RNDZ); // c2mp_temp9 = c2mp_temp12 - c2mp_temp10
  mpfr_cos(c2mp_temp8, c2mp_temp9, MPFR_RNDZ); // c2mp_temp8 = cos(c2mp_temp9)
  mpfr_pow(c2mp_temp6, c2mp_temp8, c2mp_temp7, MPFR_RNDZ); // c2mp_temp6 = pow(c2mp_temp8, c2mp_temp7)
  mpfr_sub(c2mp_temp5, c2mp_temp11, c2mp_temp6, MPFR_RNDZ); // c2mp_temp5 = c2mp_temp11 - c2mp_temp6
  mpfr_set(c2mp_temp4, c2mp_temp5, MPFR_RNDZ); // c2mp_temp4 = c2mp_temp5
  mpfr_set(c2mp_temp3, c2mp_temp12, MPFR_RNDZ); // c2mp_temp3 = c2mp_temp12
  mpfr_add(c2mp_temp1, c2mp_temp3, c2mp_temp2, MPFR_RNDZ); // c2mp_temp1 = c2mp_temp3 + c2mp_temp2
  mpfr_set(c2mp_temp0, c2mp_temp1, MPFR_RNDZ); // c2mp_temp0 = c2mp_temp1
  z = mpfr_get_d(c2mp_temp0, MPFR_RNDZ);
  y = mpfr_get_d(c2mp_temp3, MPFR_RNDZ);
  x = mpfr_get_d(c2mp_temp12, MPFR_RNDZ);

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
  mpfr_clear(c2mp_temp10);
  mpfr_clear(c2mp_temp11);
  mpfr_clear(c2mp_temp12);
  mpfr_clear(c2mp_temp13);
  mpfr_clear(c2mp_temp14);
  mpfr_clear(c2mp_temp15);
  mpfr_clear(c2mp_temp16);
  mpfr_clear(c2mp_temp17);
  mpfr_clear(c2mp_temp18);
  mpfr_clear(c2mp_temp19);
  mpfr_clear(c2mp_temp20);
  mpfr_clear(c2mp_temp21);
  mpfr_clear(c2mp_temp22);
  mpfr_clear(c2mp_temp23);
  mpfr_clear(c2mp_temp24);


  assert(x == y && y == z);
  return 0;
}
