float **A, **B, **C;
uint8_t n;

void setupMatrices() {
  initiateMatrices();
  Wire.onReceive(receiveMatrices);
  Serial.println("matrices initialized.");
}

void initiateMatrices() {
  n = devices * WORKLOAD_MAGNITUDE;
  C = malloc(n * sizeof(uint8_t *)); // allocate memory for C
  for (uint8_t i = 0; i < n; i++)
    C[i] = malloc(n * sizeof(float));
  A = malloc(n * sizeof(uint8_t *)); // allocate memory for A
  for (uint8_t i = 0; i < n; i++) {
    A[i] = malloc(n * sizeof(float));
    // A[i] = random(1.175494e-38, 3.402823e+38); //i can't figure this out. probably wont make decimals.
    //A[i] = random(1, 100);
    // Calculate decimal part of number randomly
    int decNum = random(0,99);
    float decValue = decNum / 100.0;
    // Calculate integer part of number randomly 
    int intValue = random(1,100);
  
    float finalVal = intValue + decValue;
    A[i] = &finalVal;
  }
  B = malloc(n * sizeof(uint8_t *)); // allocate memory for B
  for (uint8_t i = 0; i < n; i++) {
    B[i] = malloc(n * sizeof(float));
    // B[i] = random(1.175494e-38, 3.402823e+38);
    //B[i] = random(1, 100);
    // Calculate decimal part of number randomly
    int decNum = random(0,99);
    float decValue = decNum / 100.0;
    // Calculate integer part of number randomly 
    int intValue = random(1,100);
  
    float finalVal = intValue + decValue;
    B[i] = &finalVal;
  }
  Serial.print("generated three "); Serial.print(n); Serial.print('x'); Serial.print(n); Serial.println("matrices.");
}

void sendMatrices() {
  Serial.println("broadcasting B");
  
  Wire.beginTransmission(0);
  for (uint8_t i = 0; i < n; i++) {
    for (uint8_t j = 0; j < n; j++) {
      Wire.write((byte)&B[i][j]);
         Serial.print((byte)&A[row][i]); Serial.print('\t');
    }
    Serial.println();
  }
  Wire.endTransmission();
  delay(i2c_PROPAGATION_DELAY);

  ////---------------------------------------------------------------------- TRANSMITTING A-MATRIX BYTE BY BYTE ------- WAITING FOR MYLES TO TEST
  Serial.println("\nbroadcasting A");
  uint8_t row = 0;
  for (uint8_t addr = i2c_ADDRESS_MIN; addr < i2c_ADDRESS_MIN + devices; addr++) {
    Wire.beginTransmission(addr); /// broadcast to each device individually
    Serial.print("address "); Serial.print(addr); Serial.println(" gets:");
    for (uint8_t rows_per = 0; rows_per < WORKLOAD_MAGNITUDE; rows_per++) { ////only pass rows to each device equal to workload magnitude
      for (uint8_t i = 0; i < n; i++) {
        Wire.write((byte)&A[row][i]);  ///pass each element in row
        Serial.print((byte)&A[row][i]); Serial.print('\t');
      }
      Serial.println();
      row++;  //increment row after passing every elem in previous row
    }
    Wire.endTransmission();
    delay(i2c_PROPAGATION_DELAY);
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
