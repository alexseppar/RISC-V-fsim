int main () {
  unsigned sum = 0;
  for (unsigned i = 0; i < 997; ++i)
    sum += i;
  
  return sum == 496506 ? 0 : -1;
}
