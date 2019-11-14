/* float **A, **B, **C; //-------------------------let's get some predictability going here for a minute.
  uint8_t n; */
const uint8_t n = 3;
float A[n][n], B[n][n], C[n][n];
uint8_t received_row = 0, received_column = 0, sender_address = 0;

void setupMatrices() {
  initiateMatrices();
  Wire.onReceive(receiveMatrices);
  //Serial.println("matrices initialized.");
}

float rando() { //random float generator
  uint8_t decNum = random(0, 99); // Calculate decimal point of numbers randomly
  float decValue = decNum / 100.0;
  uint8_t intValue = random(1, 100); // Calculate integer part of numbers randomly
  float finalVal = intValue + decValue;
  return finalVal;
}

void initiateMatrices() {
  for (uint8_t i = 0; i < n; i++) {
    for (uint8_t j = 0; j < n; j++) {
      A[i][j] = rando(); //Serial.println(A[i][j]);
      B[i][j] = rando(); //Serial.println(A[i][j]);
    }
  }
  /*---------------------------------------------let's get some predictability going here for a minute.
    n = devices * WORKLOAD_MAGNITUDE;
    C = malloc(n * sizeof(float *)); // allocate memory for C
    for (uint8_t i = 0; i < n; i++)
      //C[i] = malloc(n * sizeof(float));
      C[i] = rando(); //if error here, uncomment malloc and try again
    A = malloc(n * sizeof(float *)); // allocate memory for A
    for (uint8_t i = 0; i < n; i++) {
      // A[i] = malloc(n * sizeof(float));
      A[i] = rando(); //if error here, uncomment malloc and try again
    }
    B = malloc(n * sizeof(float *)); // allocate memory for B
    for (uint8_t i = 0; i < n; i++) {
      //B[i] = malloc(n * sizeof(float));
      B[i] = rando(); //if error here, uncomment malloc and try again
    } */
  // Serial.print("generated three "); Serial.print(n); Serial.print('x'); Serial.print(n); Serial.println(" matrices.");
}

void sendMatrixB() {
  Wire.beginTransmission(0);
  uint8_t i2c_threshold = 7; //WHY WASN'T THIS 8?!
  //send entire matrix in 8 byte chunks
  for (uint8_t i = 0; i < n; i++) {
    for (uint8_t j = 0; j < n; j++) {
      if (i2c_threshold == 0) {
        Wire.endTransmission();
        Wire.beginTransmission(0);
      }
      I2C_writeAnything (B[i][j]);
      Serial.print(B[i][j]);//Serial.print(',');Seria.print(i2c_threshold);
      Serial.print('\t');
      i2c_threshold--;
      if (i2c_threshold == 255) i2c_threshold = 7; //WHY WASN'T THIS 8?!
    }
    Serial.println();
  }
  Wire.endTransmission();
  Serial.println();
}

void sendMatrixA() {
  Wire.beginTransmission(0);
  uint8_t i2c_threshold = 7; //WHY WASN'T THIS 8?!
  //send entire matrix in 8 byte chunks
  for (uint8_t i = 0; i < n; i++) {
    for (uint8_t j = 0; j < WORKLOAD_MAGNITUDE; j++) {
      if (i2c_threshold == 0) {
        Wire.endTransmission();
        Wire.beginTransmission(0);
      }
      I2C_writeAnything (A[i][j]);
      Serial.print(A[i][j]);
      //Serial.print(','); Serial.print(i2c_threshold);
      Serial.print('\t');
      i2c_threshold--;
      if (i2c_threshold == 255) i2c_threshold = 7; //WHY WASN'T THIS 8?!
    }
    Serial.println();
  }
  // Serial.println("done.");
  Wire.endTransmission();
}


/*void receiveMatrices(uint8_t howMany) {
     Serial.println("here?");
  if (sender_address == 0) { //----------------------should grab sender address----------------------
    while (1 < Wire.available()) {
      I2C_readAnything(sender_address); //THIS PROBABLY WON'T WORK AS IS caps lock
    }
    Serial.print(F("matrix received: { [address:")); Serial.print(sender_address); Serial.println(']');
  }
  else { //----------------------should grab array data----------------------
    howMany /= 4;
    while (1 < Wire.available()) {
      for (uint8_t i = received_row; i < n; i++) {
        for (uint8_t j = received_column; j < WORKLOAD_MAGNITUDE; j++) { //i think
          if (howMany > 0) {
            I2C_readAnything(A[i][j]); //Serial.print('(');Serial.print(i);Serial.print(',');Serial.print(j); Serial.print(',');Serial.print(howMany);Serial.print(')');
            Serial.print(A[i][j]); Serial.print('\t');
            received_column++;
            howMany--;
          }
        }
        if (howMany > 0) {
          Serial.println();
          received_row++;
          received_column = 0;
        }
      }
    } //Serial.print("[packet complete]");
    Serial.print(received_row); Serial.print(','); Serial.println(received_column);
    if (received_row == n - 1  && received_column == WORKLOAD_MAGNITUDE) { //if array is full as defined by opcode prameters
      received_row = 0, received_column = 0, sender_address = 0;
      Serial.println('}');
    }
  }
}*/

void receiveMatrices(int howMany) {
       Serial.println("here?");
  float result_matrix[WORKLOAD_MAGNITUDE][n]; //backwards maybe?
  if (howMany < (int)sizeof result_matrix)
    return;

  read into structure
  byte * b = (byte *) &result_matrix;
  for (byte i = 0; i < sizeof result_matrix; i++)
     b++ = Wire.read();

  Serial.println("recieved");
  for (uint8_t i = 0; i < n; i++) {
    for (uint8_t j = 0; j < n; j++) {
      Serial.print(result_matrix[i][j]); Serial.print(' ');
    }
    Serial.println();
  }
  Serial.println();
  }

/*
  void sendMatrices() {
  Serial.println("broadcasting B");

  Wire.beginTransmission(0);
  uint8_t i2c_threshold = 8;
  for (uint8_t i = 0; i < n; i++) {
    for (uint8_t j = 0; j < n; j++) {
      if (i2c_threshold == 0) {
        Wire.endTransmission();
        Wire.beginTransmission(0);
      }
      I2C_writeAnything (B[i][j]);
      i2c_threshold--;
      Serial.print(B[i][j]); Serial.print('\t');
    }
    Serial.println();
  }
  Serial.println();
  Wire.endTransmission();

  //  Wire.endTransmission();
  delay(i2c_PROPAGATION_DELAY);

  ////---------------------------------------------------------------------- TRANSMITTING A-MATRIX BYTE BY BYTE ------- WAITING FOR MYLES TO TEST
  Serial.println("\nbroadcasting A");
  uint8_t row = 0;
  for (uint8_t addr = i2c_ADDRESS_MIN; addr < i2c_ADDRESS_MIN + devices; addr++) {
    Wire.beginTransmission(addr); /// broadcast to each device individually
    Serial.print("address "); Serial.print(addr); Serial.println(" gets:");
    for (uint8_t rows_per = 0; rows_per < WORKLOAD_MAGNITUDE; rows_per++) { ////only pass rows to each device equal to workload magnitude
      for (uint8_t i = 0; i < n; i++) {
        I2C_writeAnything (A[row][i]);
        // Wire.write((byte)&A[row][i]);  ///pass each element in row
        //        Serial.print((byte)&A[row][i]); Serial.print('\t');
        Serial.print(A[row][i]); Serial.print('\t');
      }
      Serial.println();
      row++;  //increment row after passing every elem in previous row
    }
    Wire.endTransmission();
    delay(i2c_PROPAGATION_DELAY);
  }
  }*/
