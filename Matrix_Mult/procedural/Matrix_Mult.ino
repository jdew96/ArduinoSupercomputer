float rando() { //random float generator
  uint8_t decNum = random(0, 99999); // Calculate decimal point of numbers randomly
  float decValue = decNum / 100000.0;
  uint8_t intValue = random(1, 100); // Calculate integer part of numbers randomly
  float finalVal = intValue + decValue;
  return finalVal;
}

void multiplyMatrices() {
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++) {
      C[i][j] = 0;
      for (int k = 0; k < N; k++)
        C[i][j] += A[i][k] * B[k][j];
    }
}

void printMatrix() {
  Serial.println(F("C:"));
  for (uint8_t i = 0; i < N; i++) {
    for (uint8_t j = 0; j < N; j++) {
      Serial.print(C[i][j], 3);
      Serial.print('\t');
    }
    Serial.println();
  }
  Serial.println(); Serial.println();
}

void initiateMatrices() {
  //generating/printing A
  Serial.println("A:");
  for (uint8_t i = 0; i < N; i++) {
    for (uint8_t j = 0; j < N; j++) {
      A[i][j] = rando();
      Serial.print(A[i][j]); Serial.print('\t');
    }
    Serial.println();
  }
  Serial.println();
  Serial.println();

  //generating/printing B
  Serial.println("B:");
  for (uint8_t i = 0; i < N; i++) {
    for (uint8_t j = 0; j < N; j++) {
      B[i][j] = rando();
      Serial.print(B[i][j]); Serial.print('\t');
    }
    Serial.println();
  }
  Serial.println();
  Serial.println();
}
