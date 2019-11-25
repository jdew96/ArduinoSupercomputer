float rando() { //random float generator
  uint8_t decNum = random(0, 99); // Calculate decimal point of numbers randomly
  float decValue = decNum / 100.0;
  uint8_t intValue = random(1, 100); // Calculate integer part of numbers randomly
  float finalVal = intValue + decValue;
  return finalVal;
}

void sendMatrices() {
  Serial.println("sending B: "); //////////////////////////////////////////////////////
  BLA::Matrix<M_SIZE, M_SIZE> M;
  for (uint8_t i = 0; i < M_SIZE; i++) {
    for (uint8_t j = 0; j < M_SIZE; j++) {
      M(i, j) = rando();
      Serial.print(M(i, j)); Serial.print('\t');
    }
    Serial.println();
  }
  Serial.println(); Serial.println();

  uint8_t buffer_limit = 8;
  Wire.beginTransmission(0);
  for (uint8_t i = 0; i < M_SIZE; i++) {
    for (uint8_t j = 0; j < M_SIZE; j++) {
      if (buffer_limit <= 0) {
        buffer_limit = 8;
        Wire.endTransmission();
        Wire.beginTransmission(0);
      }
      I2C_writeAnything(M(i, j));
      buffer_limit--;
    }
  }
  Wire.endTransmission();
  delay(50);
  Serial.println("sending A: ");/////////////////////////////////////////////////////
  for (uint8_t i = 0; i < M_SIZE; i++) {
    for (uint8_t j = 0; j < M_SIZE; j++) {
      M(i, j) = rando();
      Serial.print(M(i, j)); Serial.print('\t');
    }
    Serial.println();
  }
  Serial.println(); Serial.println();

  uint8_t address = i2c_ADDRESS_MIN;
  buffer_limit = 8;
  uint8_t workload_threshold = 0;
  Wire.beginTransmission(address);
  for (uint8_t j = 0; j < M_SIZE; j++) {
    for (uint8_t i = 0; i < M_SIZE; i++) {
      if (buffer_limit <= 0) {
        buffer_limit = 8;
        Wire.endTransmission();
        Wire.beginTransmission(address);
      }
      I2C_writeAnything(M(i, j)); // (row, column)
      buffer_limit--;
    }
    workload_threshold++;
    if (workload_threshold == WORKLOAD_MAGNITUDE) {
      workload_threshold = 0;// Serial.println(); Serial.println(address);
      address++;
      Wire.endTransmission();
      Wire.beginTransmission(address);
    }
  }
  Wire.endTransmission();
}
