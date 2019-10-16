float **A, **B, **C;
uint8_t n = devices;
float FLOAT_CEILING = (1UL << 63);
#define someNumberTuanShouldKnow 1 ///////////////////TUAN HELP ME//////////////////////
uint8_t n_partial = n / someNumberTuanShouldKnow;


void setupMatrices() {
  initiateMatrices();
  Wire.onReceive(receiveMatrix);
  Serial.println("matrices initialized.");
}

void initiateMatrices() {
  C = malloc(n * sizeof(int *)); // allocate memory for C
  for (uint8_t i = 0; i < n; i++)
    C[i] = malloc(n * sizeof(float));
  A = malloc(n * sizeof(float *)); // allocate memory for A
  for (uint8_t i = 0; i < n; i++) {
    A[i] = malloc(n * sizeof(float));
    A[i] = random(FLOAT_CEILING);
  }
  B = malloc(n * sizeof(float *)); // allocate memory for B
  for (uint8_t i = 0; i < n; i++) {
    B[i] = malloc(n * sizeof(float));
    B[i] = random(FLOAT_CEILING);
  }
}

void sendMatrices() {
  if (n <= 0) //debug
    Serial.println("you gotta count the devices first."); //debug
  else {
    Serial.println("broadcasting A");

    uint8_t max = A[n - 1][n - 1];
    uint8_t lead = 0;
    while (max != 0) {
      max /= 10;
      lead++;
    }
    for (uint8_t i = 0; i < n; i++) {
      for (uint8_t j = 0; j < n; j++) {
        printf ("%-*i ", lead, A[i][j]);
      }
      printf("\n");
    }
    printf("\n");

    Serial.println("broadcasting B");

    max = B[n - 1][n - 1];
    lead = 0;
    while (max != 0) {
      max /= 10;
      lead++;
    }
    for (uint8_t i = 0; i < n; i++) {
      for (uint8_t j = 0; j < n; j++) {
        printf ("%-*i ", lead, B[i][j]);
      }
      printf("\n");
    }
    printf("\n");

    Wire.beginTransmission(0);
    Wire.write((byte *) &A, sizeof A); //broadcasts array A to all available devices
    delay(i2c_PROPAGATION_DELAY);
    Wire.write((byte *) &B, sizeof B); //broadcasts array B to all available devices
    Wire.endTransmission();
  }
}

void receiveMatrix(uint8_t howMany) {
  float partialMatrix[n_partial][n_partial];
  if (howMany < sizeof partialMatrix)
    return;

  // read into structure
  byte * b = (byte *) &partialMatrix;
  for (byte i = 0; i < sizeof partialMatrix; i++)
    *b++ = Wire.read();

  Serial.println("recieved");
  Serial.flush();
  uint8_t max = partialMatrix[n - 1][n - 1];
  uint8_t lead = 0;
  while (max != 0) {
    max /= 10;
    lead++;
  }
  for (uint8_t i = 0; i < n; i++) {
    for (uint8_t j = 0; j < n; j++) {
      printf ("%-*i ", lead, partialMatrix[i][j]);
    }
    printf("\n");
  }
  printf("\n");
  Serial.flush();
}
