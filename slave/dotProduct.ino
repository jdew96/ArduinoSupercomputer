float **A, **B, **C;
uint8_t n, x;

void setupMatrices() {
  initializeMatrices();
  Wire.onReceive(receiveMatrixB);
  Serial.println("matrices initialized.");
}

void initializeMatrices() {
  n = devices;
  C = malloc(workload_magnitude * sizeof(uint8_t *)); // allocate memory for C
  for (uint8_t i = 0; i < n; i++)
    C[i] = malloc(n * sizeof(float));
  A = malloc(workload_magnitude * sizeof(uint8_t *)); // allocate memory for A
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

  uint8_t max = C[n - 1][n - 1];
  uint8_t lead = 0;
  while (max != 0) {
    max /= 10;
    lead++;
  }
  for (uint8_t i = 0; i < n; i++) {
    for (uint8_t j = 0; j < n; j++) {
      printf ("%-*d ", lead, C[i][j]);
    }
    printf("\n");
  }
  printf("\n");

  Wire.beginTransmission(0);
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
        Serial.print(B[i][j]); Serial.print(' ');
      }
      Serial.println();
    }
    Serial.println();
  }

  Wire.onReceive(receiveMatrixA); //updating onReceive() to listen for A
}

void receiveMatrixA(uint8_t howMany) {
  float A[x][n]; //rows per device x total coloumns per row
  if (howMany < sizeof A)
    return;

  // read into structure
  byte * b = (byte *) &A;
  for (byte i = 0; i < sizeof A; i++)
    *b++ = Wire.read();

  //debugging--------------------------------------------
  Serial.println("received");
  uint8_t max = A[n - 1][n - 1];
  uint8_t lead = 0;
  while (max != 0) {
    max /= 10;
    lead++;
  }
  //-----------------------------------------------------

  for (uint8_t i = 0; i < n; i++) {
    for (uint8_t j = 0; j < n; j++) {
      printf ("%-*d ", lead, A[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}
