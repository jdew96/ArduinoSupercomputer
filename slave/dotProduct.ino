/*float **A, **B, **C;
  uint8_t n;*/
const uint8_t n = 3;
float A[n][n], B[n][n]; //there is no reason that A has to be n*n (other than the arduino compiler complaining about it)
uint8_t received_row = 0, received_column = 0;

//-----------------------maybe try to implement progmem arrays?-----------------------
//const float PROGMEM D[n] = {0};
//const float PROGMEM E[n] = {0};
//const float *const PROGMEM F[] = {D, E};
//------------------------------------------------------------------------------------
void setupMatrices() {
  //initializeMatrices();
  Wire.onReceive(receiveMatrixB);
  // Serial.println("matrices initialized.");
}

void multiplyAndSendMatrices() {
  int x, i, j;
  float C[workload_magnitude][n] = {0.0};
  for (i = 0; i < workload_magnitude; i++) {
    for (j = 0; j < n; j++) {
      for (x = 0; x < n; x++) {
        *(*(C + i) + j) += *(*(B + j) + x) * *(*(A + x) + i); //not sure if this array is mxn or nxm??
      }
    }
  }
  /*for (i = 0; i < n; i++) { //prints out array after received
    for (j = 0; j < workload_magnitude; j++) {
      Serial.print(C[j][i]); Serial.print('\t');
    }
    Serial.println();
    }*/

  Serial.print(F("matrix sent: { [")); Serial.print(i2c_LOCAL_ADDRESS); Serial.println(']');
  Wire.beginTransmission(i2c_LOCAL_ADDRESS);
  I2C_writeAnything (i2c_LOCAL_ADDRESS); //THIS PROBABLY WON'T WORK AS IS caps lock
  Serial.println("allegedly sent address, should be array next..");
  for (uint8_t i = 0; i < n; i++) {
    for (uint8_t j = 0; j < n; j++) {
      I2C_writeAnything (C[i][j]);
      Serial.print(C[i][j]); Serial.print('\t');
    }
    Serial.println();
  }
  Wire.endTransmission();
  Serial.println('}');
}

/*void sendMatrix() {
  Serial.println("broadcasting C");
  Wire.beginTransmission(i2c_LOCAL_ADDRESS);
  for (uint8_t i = 0; i < n; i++) {
    for (uint8_t j = 0; j < n; j++) {
      // Wire.write(i2c_LOCAL_ADDRESS); //send address at front?
      I2C_writeAnything (i2c_LOCAL_ADDRESS); //THIS PROBABLY WON'T WORK AS IS caps lock
      I2C_writeAnything (C[i][j]);
      Serial.print(C[i][j]); Serial.print(' ');
    }
    Serial.println();
  }
  Wire.endTransmission();
  Serial.println();

  // Wire.write(i2c_LOCAL_ADDRESS); //send address at front?
  // Wire.write((byte *) &C, sizeof C); //broadcasts array B to all available devices
  // delay(i2c_PROPAGATION_DELAY);
  }*/

/*
  void initializeMatrices() {
    n = devices * workload_magnitude;
  //-----------------------now initialized locally in multiplyAndSendMatrices()-----------------------
  //  C = malloc(n * sizeof(float *)); // allocate memory for C
  //  for (uint8_t i = 0; i < n; i++) {
  //    //    C[i] = malloc(n * sizeof(float));
  //    C[i][j] = 0.0; //same-same, right?
  //  }
  //-------------------------------------------------------------------------------------------
  A = malloc(n * sizeof(float *)); // allocate memory for A
  for (uint8_t i = 0; i < n; i++) {
    A[i] = malloc(workload_magnitude * sizeof(float));
  }
  B = malloc(n * sizeof(float *)); // allocate memory for B
  for (uint8_t i = 0; i < n; i++) {
    B[i] = malloc(n * sizeof(float));
  }
  }
*/

void receiveMatrixB(uint8_t howMany) {
  howMany /= 4;
  while (1 < Wire.available()) {
    for (uint8_t i = received_row; i < n; i++) {
      for (uint8_t j = received_column; j < n; j++) {
        if (howMany > 0) {
          I2C_readAnything(B[i][j]); //Serial.print('(');Serial.print(i);Serial.print(',');Serial.print(j); Serial.print(',');Serial.print(howMany);Serial.print(')');
          Serial.print(B[i][j]); Serial.print('\t');
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
  } //Serial.print("[packet complete]"); Serial.print(received_row); Serial.print(','); Serial.println(received_column);
  if (received_row == n - 1 && received_column == n) { //if array is full as defined by opcode prameters
    Serial.println(); Serial.println();
    received_row = 0, received_column = 0;
    Wire.onReceive(receiveMatrixA); //updating onReceive() to listen for A
  }
}

void receiveMatrixA(uint8_t howMany) {
  howMany /= 4;
  while (1 < Wire.available()) {
    for (uint8_t i = received_row; i < n; i++) {
      for (uint8_t j = received_column; j < workload_magnitude; j++) { //i think
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
  } //Serial.print("[packet complete]"); //Serial.print(received_row); Serial.print(','); Serial.println(received_column);
  if (received_row == n - 1  && received_column == workload_magnitude) { //if array is full as defined by opcode prameters
    received_row = 0, received_column = 0;
    Serial.println(); Serial.println();
    Wire.onReceive(receiveOpcode); //updating onReceive() to listen for next opcode
    multiplyAndSendMatrices();
  }
}

/*void receiveMatrixB(uint8_t howMany) {  //ALSO WORKS, BUT SERIALIZES ARRAY
  howMany /= 4;
  float dookie[n * n];
  while (1 < Wire.available()) { // loop through all but the last
    for (uint8_t i = received_row; i < n * n; i++) {
      if (howMany > 0) {
        I2C_readAnything(dookie[i]);
        // Serial.print('('); Serial.print(howMany); Serial.print(')');
        Serial.print(dookie[i]); Serial.print('\t');
        received_row++;
        howMany--;
      }
    }
  }
  }*/

/*void receiveMatrixB(uint8_t howMany) {
  Serial.println("Received B as:");
  while (1 < Wire.available()) { // loop through all but the last
  for (uint8_t i = 0; i < n; i++) {
    for (uint8_t j = 0; j < n; j++) {
      I2C_readAnything (B[i][j]);
      //B[i][j] = Wire.read(); // receive byte as a character
      Serial.print(B[i][j]);
      Serial.print('\t');
    }
    Serial.println();
  }
  Serial.println();
  }
  i2c_threshold--;
  if (i2c_threshold = 0)
  Wire.onReceive(receiveMatrixA); //updating onReceive() to listen for A
  }*/

/*void receiveMatrixA(uint8_t howMany) {
  Serial.println("Received A as:");
  while (1 < Wire.available()) { // loop through all but the last
    //    for (uint8_t j = 0; j < workload_magnitude; j++) {
    for (uint8_t i = 0; i < n; i++) {
      for (uint8_t j = 0; j < workload_magnitude; j++) {
        //for (uint8_t j = 0; j < n; j++) {
        I2C_readAnything (A[i][j]);
        // A[j][i] = Wire.read(); // receive byte as a character
        //        Serial.print(A[i][j]); Serial.print('\t');
      }
      //      Serial.println();
    }
    //    Serial.println();
  }
  for (uint8_t i = 0; i < n; i++) {
    for (uint8_t j = 0; j < workload_magnitude; j++) {
      Serial.print(A[i][j]); Serial.print('\t');
    }
    Serial.println();
  }
  Serial.println("here?");
  multiplyAndSendMatrices();
  Serial.println("here?");
  //sendMatrix();
  Serial.println("here?");
  Wire.onReceive(receiveOpcode);
  }*/

/*void muliplyMatrices() {
  //Serial.print("The result of multiplication is as follows: \n");
  // C = malloc(n * sizeof(float*)); // Allocating space in first dimension of array to store floats
  //float C[4][4];

  // Iterate through rows/columns
  // Result is sum of (row * column)
  //for (uint8_t i = 0; i < workload_magnitude; i++) {
  //  C[i] = malloc(n * sizeof(float*)); // Allocating space for second dimension to store floats
  //for (uint8_t j = 0; j < n; j++) {
  //  C[i][j] = 0.0; //made part of initializer for matrix
  //for (uint8_t k = 0; k < n; k++) {

  for (uint8_t a = 0; a < n; j++) {
  for (uint8_t b = 0; b < n; j++) {
    for (uint8_t c = 0; c < n; j++) {
      for (uint8_t d = 0; d < n; j++) {

      }
    }
  }
  }
  C[i][j] = A[i][k] * B[k][j] + A[i][k] * B[k][j]; // this may be an issue if we try to parse A into pieces for each slave
  Serial.print(C[i][j]); Serial.print(" = A[");
  Serial.print(i); Serial.print("]["); Serial.print(k);
  Serial.print("]*B["); Serial.print(k);
  Serial.print("]["); Serial.print(j); Serial.println(']');
  }
  //Serial.print(C[i][j]);
  //Serial.print(' ');
  }
  //Serial.print('\n');
  }
  }
*/
