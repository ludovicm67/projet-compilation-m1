#include <assert.h>
#include <stdio.h>
#include <mpfr.h>
#include <math.h>

int main(void)
{
  double x = 5, y, z;
  #pragma MPFR rounding(MPFR_RNDZ) precision(128)
  {
    // TODO(sandhose): This generates a lot of unused temporary variables
    // This should just yield an assignment, which is then be voided because of
    // symbol aliasing and dead code elimination
    x = x * 1;
    x = x + 0;
    x = x / 1;
    x = x - 0;

    // This complicated expression should generate no code, because `y` is
    // overridden after that…
    y = sin(x) - pow(cos(x - 5), 2);
    y = x;
    z = y + 0;
  }

  assert(x == y && y == z);
  return 0;
}
