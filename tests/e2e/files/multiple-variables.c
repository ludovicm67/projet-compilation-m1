#include <complex.h>
#include <stdio.h>
#include <mpc.h>
#include <math.h>

int main() {
  double complex resultat, s1, s2, s3;

  #pragma MPC precision(256) rounding(MPC_RNDZZ)
  {
    s1 = sqrt(9);
    s2 = -s1;
    s3 = 4 + 3;
    resultat = s2 * s3;
  }

  printf("==> %f%+fi\n", creal(s1), cimag(s1));
  printf("==> %f%+fi\n", creal(s2), cimag(s2));
  printf("==> %f%+fi\n", creal(s3), cimag(s3));
  printf("==> %f%+fi\n", creal(resultat), cimag(resultat));

  return 0;
}
