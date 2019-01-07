int main() {
  double x, y;
  #pragma MPFR
  {
    // This test should fail, because of the invalid `&&` operation.
    x = 2;
    y = x && (x < 3);
  }
}
