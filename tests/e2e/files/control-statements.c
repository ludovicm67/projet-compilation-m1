#include <mpfr.h>

int main(void) {
  #pragma MPFR precision(256) rounding(MPFR_RNDZZ)
  {
    // This shows that the parser correctly parse control structures, but no
    // code is generated for now. Therefore, the code generation tests are
    // skipped for this one.
    double x = 2;
    for (int i = 0; i < 10; i++) {
      if (x > 100) {
        x = 0;
        continue;
      }

      x = x * i;
    }

    for(;;) {
      x = x * 2;
      if (x < 10000) {
        x = x - 1;
      } else {
        return x;
      }
    }
  }
  return 0;
}
