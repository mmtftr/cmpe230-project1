
char *PREAMBLE_C="
  void matmul(double **mat1, int h1, int w1, double **mat2, int h2, int w2, double **result) {
    for (int i = 0; i < h1; i++) {
      for (int j = 0; j < w2; j++) {
        int sum = 0;
        for (int k = 0; k < w1; k++) {
          sum += mat1[i][k] * mat2[k][i];
        }
        result[i][j] = sum;
        
      }
    }
}
"
