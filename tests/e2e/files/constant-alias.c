#include <complex.h>
#include <stdio.h>
#include <mpc.h>
#include <math.h>

int main() {
  double complex resultat;

  #pragma MPC precision(256) rounding(MPC_RNDZZ)
  {
    // This tests for constant aliasing.
    // When generating quads, a new symbol is created for each `3`.
    // The optimizer then aliases symbols that have the same value.
    resultat = 3 * 3;
  }

  printf("==> %f%+fi\n", creal(resultat), cimag(resultat));

  return 0;
}
