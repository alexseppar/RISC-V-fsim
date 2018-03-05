void swap(int *xp, int *yp) {
  int temp = *xp;
  *xp = *yp;
  *yp = temp;
}

void bubbleSort(int arr[], int n) {
  int i, j;
  for (i = 0; i < n - 1; i++)
    for (j = 0; j < n - i - 1; j++)
      if (arr[j] > arr[j + 1])
        swap(&arr[j], &arr[j + 1]);
}

int main() {
  int arr[] = {64, 34, 25, 12, 22, 11, 90};
  int sorted[] = {11, 12, 22, 25, 34, 64, 90};
  int n = sizeof(arr) / sizeof(arr[0]);
  bubbleSort(arr, n);

  for (int i = 0; i < n; ++i)
    if (arr[i] != sorted[i])
      return -1;
  return 0;
}
