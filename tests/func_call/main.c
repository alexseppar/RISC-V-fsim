int foo() {
  return 256;
}

int main() {
  int res = foo();
  return res == 256 ? 0 : -1;
}
