float **A, **B, **C;
uint8_t n;

void setupMatrices() {
  initiateMatrices();
  Wire.onReceive(receiveMatrices);
  Serial.println("matrices initialized.");
}

void initiateMatrices() {
  n = devices;
  C = malloc(n * sizeof(uint8_t *)); // allocate memory for C
  for (uint8_t i = 0; i < n; i++)
    C[i] = malloc(n * sizeof(float));
  A = malloc(n * sizeof(uint8_t *)); // allocate memory for A
  for (uint8_t i = 0; i < n; i++) {
    A[i] = malloc(n * sizeof(float));
    // A[i] = random(1.175494e-38, 3.402823e+38); //i can't figure this out. probably wont make decimals.
    A[i] = random(1, 100);
  }
  B = malloc(n * sizeof(uint8_t *)); // allocate memory for B
  for (uint8_t i = 0; i < n; i++) {
    B[i] = malloc(n * sizeof(float));
    // B[i] = random(1.175494e-38, 3.402823e+38);
    B[i] = random(1, 100);
  }
}

void sendMatrices() {
  if (n <= 0) //debug
    Serial.println("Device list empty. Did you assign addresses?"); //debug
  else {
    Serial.println("broadcasting B");

    for (uint8_t i = 0; i < n; i++) {
      for (uint8_t j = 0; j < n; j++) {
        //Serial.print((byte)&B[i][j]); Serial.print(' ');
      }
      Serial.println();
    }
    Serial.println();

    Wire.beginTransmission(0);
    for (uint8_t i = 0; i < n; i++) {
      for (uint8_t j = 0; j < n; j++) {
        //Wire.write((byte)B[i][j]); //this works fine with hard-coded arrays
        Wire.write((byte)&B[i][j]);
      }
    }
    Wire.endTransmission();
    delay(i2c_PROPAGATION_DELAY);

    ////---------------------------------------------------------------------- TRANSMITTING A-MATRIX BYTE BY BYTE ------- WAITING FOR MYLES TO TEST
    Serial.println("broadcasting A");
    uint8_t row = 0;
    for (uint8_t addr = i2c_ADDRESS_MIN; addr < i2c_ADDRESS_MIN + devices; addr++) {
      Wire.beginTransmission(addr); /// broadcast to each device individually
      for (uint8_t rows_per = 0; rows_per < WORKLOAD_MAGNITUDE; rows_per++) { ////only pass rows to each device equal to workload magnitude
        for (uint8_t i = 0; i < n; i++) {
          Wire.write((byte)&A[row][i]);  ///pass each element in row
        }
        row++;  //increment row after passing every elem in previous row
      }
      Wire.endTransmission();
      delay(i2c_PROPAGATION_DELAY);
    }
    ////-------------------------------------------------------------------------------------------------------------------------------------------
    /*
        Serial.println("broadcasting A");

        for (uint8_t i = 0; i < n; i++) {
          for (uint8_t j = 0; j < n; j++) {
            if (i % 4 == 0) {
              Serial.print("Sending ");
              Serial.println(i + i2c_ADDRESS_MIN);
            }
            Serial.print(A[i - i2c_ADDRESS_MIN][j]); Serial.print(' ');
          }
          Serial.println();
        }
        Serial.println();

        for (uint8_t i = i2c_ADDRESS_MIN; i < n + i2c_ADDRESS_MIN; i++ ) {
          Wire.beginTransmission(i);
          for (uint8_t x = 0; x < WORKLOAD_MAGNITUDE; x++) {
          //remember, this needs to reflect the appropriate subarray size sent to each slave
            //Wire.write(A[i - i2c_ADDRESS_MIN + x]); //broadcasts partialMatrix to relevant slave
            Wire.write((byte *) &A[i - i2c_ADDRESS_MIN + x], sizeof A[i - i2c_ADDRESS_MIN + x]); //broadcasts partialMatrix to relevant slave
          }
          delay(i2c_PROPAGATION_DELAY);
          Wire.endTransmission();
        }
    */
  }
}

void receiveMatrices(int howMany) {
  float result_matrix[n * WORKLOAD_MAGNITUDE][n * WORKLOAD_MAGNITUDE]; //backwards maybe?
  if (howMany < (int)sizeof result_matrix)
    return;

  // read into structure
  byte * b = (byte *) &result_matrix;
  for (byte i = 0; i < sizeof result_matrix; i++)
    *b++ = Wire.read();

  Serial.println("recieved");
  for (uint8_t i = 0; i < n; i++) {
    for (uint8_t j = 0; j < n; j++) {
      Serial.print(result_matrix[i][j]); Serial.print(' ');
    }
    Serial.println();
  }
  Serial.println();
}
