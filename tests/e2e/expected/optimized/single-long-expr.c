#include <stdio.h>
#include <mpfr.h>
#include <math.h>

#define NMAX 42


int main() {
  int pc = 2;
  double resultat;

  
  // declaration of all variables that we will use
  mpfr_t c2mp_temp1; mpfr_init2(c2mp_temp1, 128);
  mpfr_t c2mp_temp2; mpfr_init2(c2mp_temp2, 128);
  mpfr_t c2mp_temp3; mpfr_init2(c2mp_temp3, 128);
  mpfr_t c2mp_temp4; mpfr_init2(c2mp_temp4, 128);
  mpfr_t c2mp_temp5; mpfr_init2(c2mp_temp5, 128);
  mpfr_t c2mp_temp6; mpfr_init2(c2mp_temp6, 128);
  mpfr_t c2mp_temp7; mpfr_init2(c2mp_temp7, 128);
  mpfr_t c2mp_temp8; mpfr_init2(c2mp_temp8, 128);
  mpfr_t c2mp_temp11; mpfr_init2(c2mp_temp11, 128);
  mpfr_t c2mp_temp12; mpfr_init2(c2mp_temp12, 128);
  mpfr_t c2mp_temp13; mpfr_init2(c2mp_temp13, 128);
  mpfr_t c2mp_temp14; mpfr_init2(c2mp_temp14, 128);
  mpfr_t c2mp_temp15; mpfr_init2(c2mp_temp15, 128);
  mpfr_t c2mp_temp16; mpfr_init2(c2mp_temp16, 128);
  mpfr_t c2mp_temp17; mpfr_init2(c2mp_temp17, 128);
  mpfr_t c2mp_temp18; mpfr_init2(c2mp_temp18, 128);
  mpfr_t c2mp_temp20; mpfr_init2(c2mp_temp20, 128);
  mpfr_t c2mp_temp21; mpfr_init2(c2mp_temp21, 128);
  mpfr_t c2mp_temp22; mpfr_init2(c2mp_temp22, 128);
  mpfr_t c2mp_temp23; mpfr_init2(c2mp_temp23, 128);
  mpfr_t c2mp_temp24; mpfr_init2(c2mp_temp24, 128);
  mpfr_t c2mp_temp26; mpfr_init2(c2mp_temp26, 128);
  mpfr_t c2mp_temp27; mpfr_init2(c2mp_temp27, 128); // pc
  mpfr_t c2mp_temp29; mpfr_init2(c2mp_temp29, 128);
  mpfr_t c2mp_temp30; mpfr_init2(c2mp_temp30, 128); // NMAX

  // assign values to some variables
  mpfr_set_d(c2mp_temp3, 2.000000, MPFR_RNDZ);
  mpfr_set_d(c2mp_temp14, 8.000000, MPFR_RNDZ);
  mpfr_set_d(c2mp_temp20, 4.000000, MPFR_RNDZ);
  mpfr_set_d(c2mp_temp24, 3.000000, MPFR_RNDZ);
  mpfr_set_d(c2mp_temp27, pc, MPFR_RNDZ); // pc
  mpfr_set_d(c2mp_temp30, NMAX, MPFR_RNDZ); // NMAX

  // operations
  mpfr_neg(c2mp_temp29, c2mp_temp30, MPFR_RNDZ); // c2mp_temp29 = -c2mp_temp30
  mpfr_mul(c2mp_temp26, c2mp_temp14, c2mp_temp27, MPFR_RNDZ); // c2mp_temp26 = c2mp_temp14 * c2mp_temp27
  mpfr_pow(c2mp_temp23, c2mp_temp30, c2mp_temp24, MPFR_RNDZ); // c2mp_temp23 = pow(c2mp_temp30, c2mp_temp24)
  mpfr_mul(c2mp_temp22, c2mp_temp20, c2mp_temp23, MPFR_RNDZ); // c2mp_temp22 = c2mp_temp20 * c2mp_temp23
  mpfr_sub(c2mp_temp21, c2mp_temp26, c2mp_temp22, MPFR_RNDZ); // c2mp_temp21 = c2mp_temp26 - c2mp_temp22
  mpfr_pow(c2mp_temp18, c2mp_temp30, c2mp_temp3, MPFR_RNDZ); // c2mp_temp18 = pow(c2mp_temp30, c2mp_temp3)
  mpfr_mul(c2mp_temp17, c2mp_temp20, c2mp_temp18, MPFR_RNDZ); // c2mp_temp17 = c2mp_temp20 * c2mp_temp18
  mpfr_sub(c2mp_temp16, c2mp_temp21, c2mp_temp17, MPFR_RNDZ); // c2mp_temp16 = c2mp_temp21 - c2mp_temp17
  mpfr_sub(c2mp_temp15, c2mp_temp16, c2mp_temp30, MPFR_RNDZ); // c2mp_temp15 = c2mp_temp16 - c2mp_temp30
  mpfr_sub(c2mp_temp13, c2mp_temp15, c2mp_temp14, MPFR_RNDZ); // c2mp_temp13 = c2mp_temp15 - c2mp_temp14
  mpfr_mul(c2mp_temp12, c2mp_temp29, c2mp_temp13, MPFR_RNDZ); // c2mp_temp12 = c2mp_temp29 * c2mp_temp13
  mpfr_sqrt(c2mp_temp11, c2mp_temp12, MPFR_RNDZ); // c2mp_temp11 = sqrt(c2mp_temp12)
  mpfr_pow(c2mp_temp8, c2mp_temp30, c2mp_temp3, MPFR_RNDZ); // c2mp_temp8 = pow(c2mp_temp30, c2mp_temp3)
  mpfr_mul(c2mp_temp7, c2mp_temp3, c2mp_temp8, MPFR_RNDZ); // c2mp_temp7 = c2mp_temp3 * c2mp_temp8
  mpfr_sub(c2mp_temp6, c2mp_temp11, c2mp_temp7, MPFR_RNDZ); // c2mp_temp6 = c2mp_temp11 - c2mp_temp7
  mpfr_sub(c2mp_temp5, c2mp_temp6, c2mp_temp30, MPFR_RNDZ); // c2mp_temp5 = c2mp_temp6 - c2mp_temp30
  mpfr_neg(c2mp_temp4, c2mp_temp5, MPFR_RNDZ); // c2mp_temp4 = -c2mp_temp5
  mpfr_mul(c2mp_temp2, c2mp_temp3, c2mp_temp30, MPFR_RNDZ); // c2mp_temp2 = c2mp_temp3 * c2mp_temp30
  mpfr_div(c2mp_temp1, c2mp_temp4, c2mp_temp2, MPFR_RNDZ); // c2mp_temp1 = c2mp_temp4 / c2mp_temp2
  resultat = mpfr_get_d(c2mp_temp1, MPFR_RNDZ);

  // free memory of all variables that we used
  mpfr_clear(c2mp_temp1);
  mpfr_clear(c2mp_temp2);
  mpfr_clear(c2mp_temp3);
  mpfr_clear(c2mp_temp4);
  mpfr_clear(c2mp_temp5);
  mpfr_clear(c2mp_temp6);
  mpfr_clear(c2mp_temp7);
  mpfr_clear(c2mp_temp8);
  mpfr_clear(c2mp_temp11);
  mpfr_clear(c2mp_temp12);
  mpfr_clear(c2mp_temp13);
  mpfr_clear(c2mp_temp14);
  mpfr_clear(c2mp_temp15);
  mpfr_clear(c2mp_temp16);
  mpfr_clear(c2mp_temp17);
  mpfr_clear(c2mp_temp18);
  mpfr_clear(c2mp_temp20);
  mpfr_clear(c2mp_temp21);
  mpfr_clear(c2mp_temp22);
  mpfr_clear(c2mp_temp23);
  mpfr_clear(c2mp_temp24);
  mpfr_clear(c2mp_temp26);
  mpfr_clear(c2mp_temp27);
  mpfr_clear(c2mp_temp29);
  mpfr_clear(c2mp_temp30);


  printf("==> %f\n", resultat);

  return 0;
}
