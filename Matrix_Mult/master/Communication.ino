void assignAddresses() {
  unsigned char data = i2c_ADDRESS_MIN; //i2c useable adddress range: 8 - 119

  Serial.print("Addressing i2c devices starting with 0x");
  Serial.print(data, HEX);
  Serial.println("...");

  altSerial.write(data);
}

uint8_t countDevices() {
  uint8_t devices = 0;
  Serial.println ("Scanning i2C net for devices...");

  for (uint8_t i = i2c_ADDRESS_MIN; i < i2c_ADDRESS_MAX; i++) {
    Wire.beginTransmission(i);

    if (Wire.endTransmission() == 0) {
      Serial.print ("Found address: ");
      Serial.print (i, DEC);
      Serial.print (" (0x");
      Serial.print (i, HEX);
      Serial.println (")");
      devices++;
    } //end of good response

    delay (1); //give devices time to recover
  }
  return devices;
}

void getCPUtime() { //gets cpu times from each slave after procedure
  //ask everyone for cputime
  for (uint8_t i = i2c_ADDRESS_MIN; i < i2c_ADDRESS_MIN + N; i++) {
    Wire.requestFrom(i, sizeof(CPU_time)); //request cpu time
    while (Wire.available()) {
      unsigned long single_computation_time;
      I2C_readAnything(single_computation_time);
      // Serial.print(single_computation_time);
      CPU_time += single_computation_time;
    }
    delayMicroseconds(i2c_PROPAGATION_DELAY);
  }
}

void receiveC(uint8_t howMany) {
  done_transmitting_a = true;
  while (Wire.available () > 0) {
    uint8_t buffer_limit = datatype_size;
    for (uint8_t i = 0; i < N; i++) {
      for (uint8_t j = 0; j < X; j++) {
        if (buffer_limit != 0) {
          I2C_readAnything(C[i][iter_x]);
          buffer_limit--;
        }
      }
      if (i == N - 1)
        iter_x++;
    }
    //if final value of matrix is populated with anything other than default value
        if (C[N - 1][ N - 1] != 0.0) {
//    if (C[N - 1][2] != 0.0) { //DEBUG ONLY///////////////////
      endTime = millis();
      done_receiving_c = true;
    }
  }
}

void sendData() {
  //generating/printing A
  Serial.println("A:");
  float A[N][N];
  for (uint8_t i = 0; i < N; i++) {
    for (uint8_t j = 0; j < N; j++) {
      A[i][j] = rando();
      Serial.print(A[i][j]); Serial.print('\t');
    }
    Serial.println();
  }
  Serial.println();
  Serial.println();

  //generating/printing B
  Serial.println("B:");
  float B[N][N];
  for (uint8_t i = 0; i < N; i++) {
    for (uint8_t j = 0; j < N; j++) {
      B[i][j] = rando();
      Serial.print(B[i][j]); Serial.print('\t');
    }
    Serial.println();
  }
  Serial.println();
  Serial.println();

  startTime = millis();

  //sending B via broadcast
  uint8_t buffer_limit = datatype_size;
  Wire.beginTransmission(0);
  for (uint8_t i = 0; i < N; i++) {
    for (uint8_t j = 0; j < N; j++) {
      if (buffer_limit <= 0) {
        buffer_limit = datatype_size;
        Wire.endTransmission();
        Wire.beginTransmission(0);
      }
      I2C_writeAnything(B[i][j]);
      buffer_limit--;
    }
  }
  Wire.endTransmission();
  delayMicroseconds(i2c_PROPAGATION_DELAY);

  //sending A via individual send
  uint8_t address = i2c_ADDRESS_MIN;
  buffer_limit = datatype_size;
  uint8_t workload_threshold = 0;
  Wire.beginTransmission(address);
  for (uint8_t j = 0; j < N; j++) {
    for (uint8_t i = 0; i < N; i++) {
      if (buffer_limit <= 0) {
        buffer_limit = datatype_size;
        Wire.endTransmission();
        Wire.beginTransmission(address);
      }
      I2C_writeAnything(A[i][j]); // (row, column)
      buffer_limit--;
    }
    workload_threshold++;
    if (workload_threshold == X) {
      workload_threshold = 0;//Serial.println(); //Serial.println(address);
      address++;
      Wire.endTransmission();
      Wire.beginTransmission(address);
    }
  }
  Wire.endTransmission();
  iter_x = 0, iter_y = 0;
}

void Signal() {
//  Serial.println("Signaling...");
  while (!done_transmitting_a) {
    altSerial.write('*');
  }
}
