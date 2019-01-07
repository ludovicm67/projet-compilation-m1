#include <stdio.h>
#include <mpfr.h>

int main(void)
{
  double x, y = 2;
  #pragma MPFR
  {
    // This should generate a warning because the compiler can't infer the type of `y`
    x = y;
    x = 5;
  }

  printf("%f, %f", x, y);
  return 0;
}
