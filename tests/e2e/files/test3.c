#include <complex.h>
#include <stdio.h>
#include <mpc.h>
#include <math.h>

int main() {
  double complex resultat;

  #pragma MPC precision(256) rounding(MPC_RNDZZ)
  {
    resultat = 3 * 3;
  }

  printf("==> %f%+fi\n", creal(resultat), cimag(resultat));

  return 0;
}
