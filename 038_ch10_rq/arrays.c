#include <stdlib.h>

void initArray1(size_t nr, size_t nc, int arr[][nc]) {
  for (size_t i = 0; i < nr; i++) {
    for (size_t j = 0; j < nc; j++) {
      arr[i][j] = i*nc + j + 1;
    }
  }
}

void initArray2(size_t nr, size_t nc, int * arr[]) {
  for (size_t i = 0; i < nr; i++) {
    for (size_t j = 0; j < nc; j++) {
      arr[i][j] = i*nc + j + 1;
    }
  }
}

int main(void) {
  size_t nrows = 2;
  size_t ncols = 3;
  int arr1[nrows][ncols];
  initArray1(nrows, ncols, arr1);
  int row1[ncols];
  int row2[ncols];
  int * arr2[] = {row1, row2};
  initArray2(nrows, ncols, arr2);
  return EXIT_SUCCESS;
}
