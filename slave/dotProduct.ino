float **A, **B, **C;
uint8_t n, x; //do we actually use x?

void setupMatrices() {
  initializeMatrices();
  Wire.onReceive(receiveMatrixB);
  Serial.println("matrices initialized.");
}

void initializeMatrices() {
  n = devices * workload_magnitude; //nolonger reflects number of devices
  C = malloc(n * sizeof(uint8_t *)); // allocate memory for C
  for (uint8_t i = 0; i < n; i++)
    C[i] = malloc(n * sizeof(float));
  A = malloc(n * sizeof(uint8_t *)); // allocate memory for A
  for (uint8_t i = 0; i < n; i++) {
    A[i] = malloc(n * sizeof(float));
  }
  B = malloc(n * sizeof(uint8_t *)); // allocate memory for B
  for (uint8_t i = 0; i < n; i++) {
    B[i] = malloc(n * sizeof(float));
  }
}

void dotProduct() {
  //fancy dot-product math
  C = B; //definitely temporary
}

void sendMatrix() {
  Serial.println("broadcasting C");

  for (uint8_t i = 0; i < n; i++) {
    for (uint8_t j = 0; j < n; j++) {
      B[i][j] = Wire.read(); // receive byte as a character
      Serial.print(C[i][j]); Serial.print(' ');
    }
    Serial.println();
  }
  Serial.println();

  Wire.beginTransmission(0);
  // Wire.write(i2c_LOCAL_ADDRESS); //send address at front?
  Wire.write((byte *) &C, sizeof C); //broadcasts array B to all available devices
  delay(i2c_PROPAGATION_DELAY);
  Wire.endTransmission();
}

void receiveMatrixB(uint8_t howMany) {
  Serial.println("Received B as:");
  while (1 < Wire.available()) { // loop through all but the last
    for (uint8_t i = 0; i < n; i++) {
      for (uint8_t j = 0; j < n; j++) {
        B[i][j] = Wire.read(); // receive byte as a character
        Serial.print(B[i][j]); Serial.print('\t');
      }
      Serial.println();
    }
    Serial.println();
  }

  Wire.onReceive(receiveMatrixA); //updating onReceive() to listen for A
}

void receiveMatrixA(uint8_t howMany) {
  Serial.println("Received A as:");
  while (1 < Wire.available()) { // loop through all but the last
    for (uint8_t j = 0; j < workload_magnitude; j++) {
      for (uint8_t i = 0; i < n; i++) {
        //for (uint8_t j = 0; j < n; j++) {
        A[j][i] = Wire.read(); // receive byte as a character
        Serial.print(A[j][i]); Serial.print('\t');
      }
      Serial.println();
    }
    Serial.println();
  }

  Wire.onReceive(receiveOpcode);
}
