unsigned factorial(unsigned n);

int main() {
  unsigned res = factorial(9);
  return res == 362880 ? 0 : -1;
}


unsigned factorial(unsigned n) {
  if (n <= 1)
    return 1;
  return n * factorial(n - 1);
}
