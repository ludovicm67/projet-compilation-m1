#include <stdio.h>
#include <mpfr.h>
#include <math.h>

#define NMAX 42


int main() {
  int pc = 2;
  double resultat;

  #pragma MPFR precision(128) rounding(MPFR_RNDZ)
  {
    // This tests the long expression that was defined in the assignments subject
    resultat = -(sqrt(-NMAX*(8*pc-4*pow(NMAX,3)-4*pow(NMAX,2)-NMAX-8))-2*pow(NMAX,2)-NMAX)/(2*NMAX);
  }

  printf("==> %f\n", resultat);

  return 0;
}
