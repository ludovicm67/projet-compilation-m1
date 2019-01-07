#include <stdio.h>
#include <complex.h>
#include <mpc.h>
#include <mpfr.h>
#include <math.h>

int main(void) {
  double x = 2;
  #pragma MPFR
  x = x * 2;

  double complex y = 2;
  #pragma MPC
  y = y * 2;

  printf("x: %f, y: %f%+f\n", x, crealf(y), cimagf(y));
}
