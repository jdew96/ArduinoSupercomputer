BLA::Matrix<M_SIZE, M_SIZE> B;
BLA::Matrix < M_SIZE, M_SIZE / M_SIZE > A;
uint8_t iter_x = 0, iter_y = 0;
void setupMatrices() {
  Wire.onReceive(receiveMatrixB);
}

void multiplyAndSendMatrices() {
  BLA::Matrix < M_SIZE, M_SIZE / M_SIZE > M = B * A;
  Serial.println("sending C: ");///////////////////////////////////////////////////////
  for (uint8_t i = iter_y; i < M_SIZE; i++) {
    for (uint8_t j = iter_x; j < workload_magnitude; j++) {
      Serial.print(M(i, j)); Serial.print('\t');
    }
    Serial.println();
  }
  Serial.println(); Serial.println();

  uint8_t buffer_limit = 8;
  Wire.beginTransmission(i2c_LOCAL_ADDRESS);
  for (uint8_t i = iter_y; i < M_SIZE; i++) {
    for (uint8_t j = iter_x; j < workload_magnitude; j++) {
      if (buffer_limit <= 0) {
        buffer_limit = 8;
        Wire.endTransmission();
        Wire.beginTransmission(i2c_LOCAL_ADDRESS);
      }
      I2C_writeAnything(M(i, j)); // (row, column)
      buffer_limit--;
    }
  }
  Wire.endTransmission();
}

void receiveMatrixB(uint8_t howMany) {
  while (Wire.available () > 0) {
    if (iter_x == 0 && iter_y == 0)
      Serial.print("reading B:");////////////////////////////////////////////////////////
    Serial.println();
    uint8_t buffer_limit = 8;
    for (uint8_t i = iter_y; i < M_SIZE; i++) {
      for (uint8_t j = iter_x; j < M_SIZE; j++) {
        if (buffer_limit != 0) {
          I2C_readAnything(B(iter_y, iter_x));
          Serial.print(B(i, j));
          Serial.print('\t');
          buffer_limit--;
          iter_x++;
          if (iter_x >= M_SIZE) {
            iter_x = 0;
            iter_y++;
          }
        }
      }
      if (buffer_limit != 0)
        Serial.println();
    } //Serial.print(iter_x); Serial.print(','); Serial.println(iter_y);
    if (iter_y == M_SIZE ) {
      iter_x = 0, iter_y = 0;
      Serial.println();
      Wire.onReceive(receiveMatrixA);
    }
  }
}

void receiveMatrixA(uint8_t howMany) {
  while (Wire.available () > 0) {
    if (iter_x == 0 && iter_y == 0)
      Serial.print("reading A:");////////////////////////////////////////////////////////
    Serial.println();
    uint8_t buffer_limit = 8;
    for (uint8_t i = iter_y; i < M_SIZE; i++) {
      for (uint8_t j = iter_x; j < workload_magnitude; j++) {
        if (buffer_limit != 0) {
          I2C_readAnything(A(iter_y, iter_x));
          Serial.print(A(i, j));
          Serial.print('\t');
          buffer_limit--;
          iter_x++;
          if (iter_x >= workload_magnitude)
            iter_x = 0;
        }
      }
      if (buffer_limit != 0) {
        iter_y++;
        Serial.println();
      }
    } //Serial.print(iter_x);Serial.print(',');Serial.println(iter_y);
//    if (iter_y == M_SIZE) {
//      Wire.onReceive(receiveOpcode);
//      multiplyAndSendMatrices();
//    }
  }
}
