float rando() { //random float generator, courtesy of JackNotJohn
  uint8_t decNum = random(0, 99); // Calculate decimal point of numbers randomly
  float decValue = decNum / 100.0;
  uint8_t intValue = random(1, 100); // Calculate integer part of numbers randomly
  float finalVal = intValue + decValue;
  return finalVal;
}

void sendMatrices() { //sends matrix B and A to slaves
  Serial.println("sending B: ");
  BLA::Matrix<M_SIZE, M_SIZE> M; //instantiate matrix locally to save memory
  for (uint8_t i = 0; i < M_SIZE; i++) { //iterate row
    for (uint8_t j = 0; j < M_SIZE; j++) { //iterate column
      M(i, j) = rando(); //assign index random value
      Serial.print(M(i, j)); Serial.print('\t');
    }
    Serial.println();
  }
  Serial.println(); Serial.println();

  uint8_t buffer_limit = 8; //keeps track of how many bytes have been added to the send buffer
  Wire.beginTransmission(0); //wire send to all devices simultaneously
  for (uint8_t i = 0; i < M_SIZE; i++) { //iterate row
    for (uint8_t j = 0; j < M_SIZE; j++) { //iterate column
      if (buffer_limit <= 0) { //if send buffer is filled with data
        buffer_limit = 8; //reset buffer counter
        Wire.endTransmission(); //end wire sending
        Wire.beginTransmission(0); //restart wire sending to all devices simultaneously
      }
      I2C_writeAnything(M(i, j)); //write B(row, column)
      buffer_limit--; //count down to keep track of remaining space in send buffer
    }
  }
  Wire.endTransmission(); //final end of wire sending when matrix traversal is complete
  delay(i2c_PROPAGATION_DELAY); //i2c propagation delay
  Serial.println("sending A: "); 
  for (uint8_t i = 0; i < M_SIZE; i++) { //row iteration
    for (uint8_t j = 0; j < M_SIZE; j++) { //column iteration
      M(i, j) = rando(); //assign index random value
      Serial.print(M(i, j)); Serial.print('\t');
    }
    Serial.println();
  }
  Serial.println(); Serial.println();

  uint8_t address = i2c_ADDRESS_MIN; //start iterating through slave addresses to send data
  buffer_limit = 8; //keeps track of how many bytes have been added to the send buffer
  uint8_t workload_threshold = 0; //keeps track of how many columns we pass to each slave
  Wire.beginTransmission(address); //starts adding data to 32 byte wire send buffer 
  for (uint8_t j = 0; j < M_SIZE; j++) { //iterate rows
    for (uint8_t i = 0; i < M_SIZE; i++) { //iterate columns
      if (buffer_limit <= 0) { //if send buffer is filled with data
        buffer_limit = 8; //reset buffer counter
        Wire.endTransmission(); //end wire sending
        Wire.beginTransmission(address); //restart wire sending
      }
      I2C_writeAnything(M(i, j)); // (row, column)
      buffer_limit--; //write C(row, column)
    }
    workload_threshold++; //increment column based off of number of columns sent to each slave
    if (workload_threshold == WORKLOAD_MAGNITUDE) { //if number of columns for slave has been sent
      workload_threshold = 0;// Serial.println(); Serial.println(address); //reset counter for number of columns to pass to each slave
      address++; //increment to next slave adddress to receive next chunk of matrix A
      Wire.endTransmission(); //end wire sending
      Wire.beginTransmission(address); //restart wire sending on next address
    }
  }
  Wire.endTransmission(); //final end of wire sending when matrix traversal is complete
}
