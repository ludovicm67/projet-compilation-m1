int main() {
  double x, y;
  #pragma MPFR
  {
    x = 2;
    y = x && (x < 3);
  }
}
