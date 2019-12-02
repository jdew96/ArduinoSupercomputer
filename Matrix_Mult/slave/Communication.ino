void assignAddresses() {
  if (i2c_LOCAL_ADDRESS != 0) {
    unsigned char data = i2c_LOCAL_ADDRESS + 1; //i2c useable adddress range: 8 - 119

    Serial.print("Addressed next device i2c with 0x");
    Serial.println(data, HEX);

    altSerial.write(data);
  }
}

void receiveAddress() {
  Serial.println("Waiting on i2c address...");
  unsigned char input;
  while (i2c_LOCAL_ADDRESS == 0) {
    input = altSerial.read();
    if (input >= i2c_ADDRESS_MIN && input <= i2c_ADDRESS_MAX) {
      Serial.print("i2c address set to 0x");
      Serial.println(input, HEX);
      i2c_LOCAL_ADDRESS = input;
      Wire.setClock(i2c_CLOCK); //full throttle for i2c, check for stability when enabling
      Wire.begin(i2c_LOCAL_ADDRESS);
      TWAR = (i2c_LOCAL_ADDRESS << 1) | 1;  // enable broadcasts to be received
    }
    else {
      if (input != 255) {
        Serial.print("i2c address 0x");
        Serial.print(input, HEX);
        Serial.println(" invalid.");
      }
    }
  }
  if (i2c_LOCAL_ADDRESS < i2c_ADDRESS_MAX)
    assignAddresses();
}

void giveCPUtime() {
  I2C_writeAnything(CPU_time);
}

void receiveMatrixB(uint8_t howMany) {
  while (Wire.available () > 0) {
    uint8_t buffer_limit = datatype_size;
    for (uint8_t i = iter_y; i < N; i++) {
      for (uint8_t j = iter_x; j < N; j++) {
        if (buffer_limit != 0) {
          I2C_readAnything(B[iter_y][iter_x]);
          buffer_limit--;
          iter_x++;
          if (iter_x >= N) {
            iter_x = 0;
            iter_y++;
          }
        }
      }
    }
  }
  if (B[N - 1][N - 1] != 0.0) {
    Serial.println("Received B");
    done_receiving_b = true;
  }
}

void receiveMatrixA(uint8_t howMany) {
  while (Wire.available () > 0) {
    uint8_t buffer_limit = datatype_size;
    for (uint8_t i = iter_y; i < N; i++) {
      for (uint8_t j = iter_x; j < X; j++) {
        if (buffer_limit != 0) {
          I2C_readAnything(A[iter_y][iter_x]);
          buffer_limit--;
          iter_x++;
          if (iter_x >= X)
            iter_x = 0;
        }
      }
      if (buffer_limit != 0) {
        iter_y++;
      }
    }
    //if final value of matrix is populated with anything other than default value
    if (A[N - 1][X - 1] != 0.0) {
      Serial.println("Received A");
      done_receiving_a = true;
    }
  }
}

void sendData() {
  uint8_t buffer_limit = datatype_size;
  Wire.beginTransmission(i2c_MASTER_ADDRESS);
  for (uint8_t i = iter_y; i < N; i++) {
    for (uint8_t j = iter_x; j < X; j++) {
      if (buffer_limit <= 0) {
        buffer_limit = datatype_size;
        Wire.endTransmission();
        Wire.beginTransmission(i2c_MASTER_ADDRESS);
      }
      I2C_writeAnything(C[i][j]); // (row, column)
      buffer_limit--;
    }
  }
  Wire.endTransmission();
  Serial.println("Sent C");
  printMatrix('C');
}

void Signal() {
//    Serial.println("Signaling...");
  altSerial.write('*');
}

void Wait() {
  char c = '0';
//    Serial.println("Listening...");
  while (c != '*') {
    c = altSerial.read();
  }
//  Serial.println(c);
}
