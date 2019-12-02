void multiplyMatrices() {
  unsigned long start;
  unsigned long finish;
  start = millis();
  //  C = B * A;
  int x, i, j;
  for (i = 0; i < X; i++) {
    for (j = 0; j < N; j++) {
      for (x = 0; x < N; x++) {
        *(*(C + i) + j) += *(*(B + j) + x) * *(*(A + x) + i); 
      }
    }
  }
  finish = millis();
  CPU_time = finish - start;
}

void printMatrix(char m) {
  uint8_t column_limit = X;
  if (m == 'B')
    column_limit = N;

  Serial.print(m); Serial.println(':');
  for (uint8_t i = iter_y; i < N; i++) {
    for (uint8_t j = iter_x; j < column_limit; j++) {
      if (m == 'A')
        Serial.print(A[i][j]);
      else if (m == 'B')
        Serial.print(B[i][j]);
      else if (m == 'C')
        Serial.print(C[i][j]);
      Serial.print('\t');
    }
    Serial.println();
  }
  Serial.println(); Serial.println();
}
