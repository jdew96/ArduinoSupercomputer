uint8_t iter_x = 0, iter_y = 0;
BLA::Matrix < M_SIZE, M_SIZE> C; //eventually we might be able to remove this entirely

float rando() { //random float generator
  uint8_t decNum = random(0, 99); // Calculate decimal point of numbers randomly
  float decValue = decNum / 100.0;
  uint8_t intValue = random(1, 100); // Calculate integer part of numbers randomly
  float finalVal = intValue + decValue;
  return finalVal;
}

void sendMatrices() {
  //Serial.println("sending B: "); //////////////////////////////////////////////////////
  BLA::Matrix<M_SIZE, M_SIZE> M;
  for (uint8_t i = 0; i < M_SIZE; i++) {
    for (uint8_t j = 0; j < M_SIZE; j++) {
      M(i, j) = rando();
      //Serial.print(M(i, j)); //Serial.print('\t');
    }
    //Serial.println();
  }
  //Serial.println();
  //  //Serial.println();

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
  delay(i2c_PROPAGATION_DELAY);
  //Serial.println("sending A: ");/////////////////////////////////////////////////////
  for (uint8_t i = 0; i < M_SIZE; i++) {
    for (uint8_t j = 0; j < M_SIZE; j++) {
      M(i, j) = rando();
      //Serial.print(M(i, j)); //Serial.print('\t');
    }
    //Serial.println();
  }
  //Serial.println(); 
//  //Serial.println();

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
      workload_threshold = 0;// //Serial.println(); //Serial.println(address);
      address++;
      Wire.endTransmission();
      Wire.beginTransmission(address);
    }
  }
  Wire.endTransmission();
  iter_x = 0, iter_y = 0; //i think this is important
  informNeighbor();  //lets the dummy informNeighbor know it can trigger now
}

/////////////untested for matrices greater than 8*8///////////////////////
void receiveC(uint8_t howMany) {
  a_transmission_complete = true;
  while (Wire.available () > 0) {
    if (C(0, 0) == 0.0) {
      //Serial.println("reading C:");
    }
    uint8_t buffer_limit = 8;
    for (uint8_t i = 0; i < M_SIZE; i++) {
      for (uint8_t j = 0; j < WORKLOAD_MAGNITUDE; j++) {
        if (buffer_limit != 0) {
          I2C_readAnything(C(i, iter_x));
          buffer_limit--;
        }
      } // if (buffer_limit != 0)
      if (i == M_SIZE - 1)
        iter_x++;
    }
    //if final value of matrix is populated with anything other than default value
    if (C(M_SIZE - 1, M_SIZE - 1) != 0.0) {
      for (uint8_t i = 0; i < M_SIZE; i++) {
        for (uint8_t j = 0; j < M_SIZE; j++) {
          //Serial.print(C(i, j), 3); //Serial.print('\t');
        }
        //Serial.println();
      }
      //Serial.println();
      done_receiving_c = true;
    }
  }
}

/* void receiveC(uint8_t howMany) {
  a_transmission_complete = true;
  //Serial.println(F("received part from slave"));
  float result_matrix[M_SIZE][WORKLOAD_MAGNITUDE]; //backwards maybe?
  if (howMany < (int)sizeof result_matrix)
    return;

  //  read into structure
  byte * b = (byte *) &result_matrix;
  for (byte i = 0; i < sizeof result_matrix; i++)
      b++ = Wire.read();

  //Serial.println("recieved");
  for (uint8_t i = 0; i < M_SIZE; i++) {
    for (uint8_t j = 0; j < WORKLOAD_MAGNITUDE; j++) {
      //Serial.print(result_matrix[i][j]); //Serial.print(' ');
    }
    //Serial.println();
  }
  //Serial.println();
  }*/
