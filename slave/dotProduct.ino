BLA::Matrix<M_SIZE, M_SIZE> B; //placeholder for received array B
BLA::Matrix < M_SIZE, M_SIZE / M_SIZE > A;  //placeholder for received array A 
uint8_t iter_x = 0, iter_y = 0; //keeps track of iterating through received arrays since the function is called every 8 bytes
void setupMatrices() {
  Wire.onReceive(receiveMatrixB);
}

void multiplyAndSendMatrices() {
  BLA::Matrix < M_SIZE, M_SIZE / M_SIZE > M = B * A; //resultant matrix of multiplication
  Serial.println("sending C: ");///////////////////////////////////////////////////////
  for (uint8_t i = iter_y; i < M_SIZE; i++) { //row iteration
    for (uint8_t j = iter_x; j < workload_magnitude; j++) { //column iteration
      Serial.print(M(i, j)); Serial.print('\t');
    }
    Serial.println();
  }
  Serial.println(); Serial.println();

  uint8_t buffer_limit = 8; //keeps track of how many bytes have been added to the send buffer
  Wire.beginTransmission(i2c_LOCAL_ADDRESS); //starts adding data to 32 byte wire send buffer 
  for (uint8_t i = iter_y; i < M_SIZE; i++) { //row iteration
    for (uint8_t j = iter_x; j < workload_magnitude; j++) { //column iteration (smaller because of slave workload delegation)
      if (buffer_limit <= 0) { //if send buffer is filled with data
        buffer_limit = 8; //reset buffer counter
        Wire.endTransmission(); //end wire sending
        Wire.beginTransmission(i2c_LOCAL_ADDRESS); //restart wire sending
      }
      I2C_writeAnything(M(i, j)); //write C(row, column)
      buffer_limit--; //count down to keep track of remaining space in send buffer
    }
  }
  Wire.endTransmission(); //final end of wire sending when matrix traversal is complete
}

void receiveMatrixB(uint8_t howMany) { //this receives matrix data in 8 byte increments
  while (Wire.available () > 0) { //necessary to hear the entire matrix being passed
    if (iter_x == 0 && iter_y == 0) //run-once conditional to preface matrix printout with label
      Serial.print("reading B:");
    Serial.println();
    uint8_t buffer_limit = 8; //keeps track of how many bytes have been added to the receive buffer
    for (uint8_t i = iter_y; i < M_SIZE; i++) { //row iteration
      for (uint8_t j = iter_x; j < M_SIZE; j++) { //column iteration
        if (buffer_limit != 0) { //if receive buffer is NOT filled with data yet
          I2C_readAnything(B(iter_y, iter_x)); //read B(row, column)
          Serial.print(B(i, j));
          Serial.print('\t');
          buffer_limit--; //count down to keep track of remaining space in receive buffer
          iter_x++; //increment column
          if (iter_x >= M_SIZE) { //if we reach the end of the column
            iter_x = 0; //reset global x iterator to 0
            iter_y++; //iterate row
          }
        }
      }
      if (buffer_limit != 0) //if receive buffer is NOT filled with data yet
        Serial.println(); //print a newline just so it makes sense in the console
    } //Serial.print(iter_x); Serial.print(','); Serial.println(iter_y);
    if (iter_y == M_SIZE ) {  //run-once conditional to escape matrix B reception and change listener command 
      iter_x = 0, iter_y = 0; //resets global matrix iterators
      Serial.println();
      Wire.onReceive(receiveMatrixA);
    }
  }
}

void receiveMatrixA(uint8_t howMany) { //this receives matrix data in 8 byte increments
  while (Wire.available () > 0) { //necessary to hear the entire matrix being passed
    if (iter_x == 0 && iter_y == 0) //run-once conditional to preface matrix printout with label
      Serial.print("reading A:");
    Serial.println();
    uint8_t buffer_limit = 8; //keeps track of how many bytes have been added to the receive buffer
    for (uint8_t i = iter_y; i < M_SIZE; i++) { //row iteration
      for (uint8_t j = iter_x; j < workload_magnitude; j++) { //column iteration
        if (buffer_limit != 0) { //if receive buffer is NOT filled with data yet
          I2C_readAnything(A(iter_y, iter_x)); //read A(row, column)
          Serial.print(A(i, j));
          Serial.print('\t');
          buffer_limit--; //count down to keep track of remaining space in receive buffer
          iter_x++; //increment column
          if (iter_x >= workload_magnitude) //if we reach the end of the column
            iter_x = 0; //reset global x iterator to 0
        }
      }
      if (buffer_limit != 0) { //if receive buffer is NOT filled with data yet
        iter_y++; //iterate row
        Serial.println(); //print a newline just so it makes sense in the console
      }
    } //Serial.print(iter_x);Serial.print(',');Serial.println(iter_y);
////////////////////// THE PART I CAN'T GET TO WORK ////////////////////////
//    if (iter_y == M_SIZE) { //run-once conditional to escape matrix A reception and change listener command 
//      Wire.onReceive(receiveOpcode); 
//      multiplyAndSendMatrices();
//    }
  }
}
