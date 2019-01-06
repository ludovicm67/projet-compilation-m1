#include <stdio.h>
#include <mpfr.h>

int main() {
  double complex resultat;
  #pragma MPC precision(256) rounding(MPC_RNDZZ)
  {
    resultat = -(sqrt(-NMAX*(8*pc-4*pow(NMAX,3)-4*pow(NMAX,2)-NMAX-8))-2*pow(NMAX,2)-NMAX)/(2*NMAX);
  }
  printf("==> %f\n", resultat);

  return 0;
}
