BLA::Matrix<M_SIZE, M_SIZE> B;
BLA::Matrix < M_SIZE, M_SIZE / M_SIZE > A;


void setupMatrices() {
  Wire.onReceive(receiveMatrixB);
}

void multiplyAndSendMatrices() {
  BLA::Matrix < M_SIZE, M_SIZE / M_SIZE > M = B * A;
  //DEBUGGING ONLY
  //  M(0, 0) = M(1, 0) = M(2, 0)= M(3, 0) = i2c_LOCAL_ADDRESS;
  ////////////////

  receiveReadySignal(); //spinlock waiting on ready signal
  Serial.println("sending C: ");
  for (uint8_t i = iter_y; i < M_SIZE; i++) {
    for (uint8_t j = iter_x; j < workload_magnitude; j++) {
      Serial.print(M(i, j)); Serial.print('\t');
    }
    Serial.println();
  }
  Serial.println(); Serial.println();

  uint8_t buffer_limit = 8;
  Wire.beginTransmission(i2c_MASTER_ADDRESS);
  for (uint8_t i = iter_y; i < M_SIZE; i++) {
    for (uint8_t j = iter_x; j < workload_magnitude; j++) {
      if (buffer_limit <= 0) {
        buffer_limit = 8;
        Wire.endTransmission();
        Wire.beginTransmission(i2c_MASTER_ADDRESS);
      }
      I2C_writeAnything(M(i, j)); // (row, column)
      buffer_limit--;
    }
  }
  Wire.endTransmission();
  informNeighbor();
}

void receiveMatrixB(uint8_t howMany) {
  while (Wire.available () > 0) {
    if (iter_x == 0 && iter_y == 0)
      Serial.print("reading B:");
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
    }
  }
  if (B(M_SIZE - 1, M_SIZE - 1) != 0.0) {
    // Serial.println();
    done_receiving_b = true;
  }
}

void receiveMatrixA(uint8_t howMany) {
  while (Wire.available () > 0) {
    if (iter_x == 0 && iter_y == 0)
      Serial.print("reading A:");
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
    }
    //        Serial.println(A(workload_magnitude - 1, M_SIZE - 1));
    //if final value of matrix is populated with anything other than default value
    if (A(M_SIZE - 1, workload_magnitude - 1) != 0.0) {
      Serial.println();
      done_receiving_a = true;
    }
  }
}
