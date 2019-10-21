uint8_t **A, **B, **C;
uint8_t n, x;
float FLOAT_CEILING = (1UL << 63);
//#define ROWS_PER_DEVICE 4

void setupMatrices() {
  initializeMatrices();
  Wire.onReceive(receiveMatrixB);
  Serial.println("matrices initialized.");
}

void initializeMatrices() {
  n = devices;
  C = malloc(n * sizeof(uint8_t *)); // allocate memory for C
  for (uint8_t i = 0; i < n; i++)
    C[i] = malloc(n * sizeof(uint8_t));
  A = malloc(workload_magnitude * sizeof(uint8_t *)); // allocate memory for A
  for (uint8_t i = 0; i < n; i++) {
    A[i] = malloc(n * sizeof(uint8_t));
  }
  B = malloc(n * sizeof(uint8_t *)); // allocate memory for B
  for (uint8_t i = 0; i < n; i++) {
    B[i] = malloc(n * sizeof(uint8_t));
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
  if (howMany < sizeof B)
    return;

  // read into structure
  byte * b = (byte *) &B;
  for (byte i = 0; i < sizeof B; i++)
    *b++ = Wire.read();

  //debugging--------------------------------------------
        Serial.println("Received B as:");
for (uint8_t i = 0; i < n; i++) {
      for (uint8_t j = 0; j < n; j++) {
        Serial.print(B[i][j]); Serial.print(' ');
      }
      Serial.println();
    }
    Serial.println();

  /*
  Serial.println("received");
  uint8_t max = B[n - 1][n - 1];
  uint8_t lead = 0;
  while (max != 0) {
    max /= 10;
    lead++;
  }
  //-----------------------------------------------------

  for (uint8_t i = 0; i < n; i++) {
    for (uint8_t j = 0; j < n; j++) {
      printf ("%-*d ", lead, B[i][j]);
    }
    printf("\n");
  }
  printf("\n");
  */
}

void receiveMatrixA(uint8_t howMany) {
  float partialMatrix[x][n]; //rows per device x total coloumns per row
  if (howMany < sizeof partialMatrix)
    return;

  // read into structure
  byte * b = (byte *) &partialMatrix;
  for (byte i = 0; i < sizeof partialMatrix; i++)
    *b++ = Wire.read();

  //debugging--------------------------------------------
  Serial.println("received");
  uint8_t max = partialMatrix[n - 1][n - 1];
  uint8_t lead = 0;
  while (max != 0) {
    max /= 10;
    lead++;
  }
  //-----------------------------------------------------

  for (uint8_t i = 0; i < n; i++) {
    for (uint8_t j = 0; j < n; j++) {
      printf ("%-*d ", lead, partialMatrix[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}
