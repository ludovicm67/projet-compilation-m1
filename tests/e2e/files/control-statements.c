#include <mpfr.h>

int main(void) {
  #pragma MFR precision(256) rouding(MPFR_RNDZZ)
  {
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
