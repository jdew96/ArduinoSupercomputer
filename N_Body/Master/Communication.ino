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

void receiveData(uint8_t size) {
  //Serial.println("Interrupt...");
  //Serial.println(size);

  if (size >= sizeof(Vector) + sizeof(Vector) + sizeof(single_computation_time)) {
    //Serial.println("Interrupt acknowledged");
    I2C_readAnything(positions[vectors_received]);

    //Serial.print("recieved: ");
    //printVector(positions[vectors_received]);
    vectors_received++;

    I2C_readAnything(positions[vectors_received]);

    //Serial.print("recieved: ");
    //printVector(positions[vectors_received]);
    vectors_received++;

    I2C_readAnything(single_computation_time);
    if(single_computation_time > worst_computation_time){
      worst_computation_time = single_computation_time;
    }
    
  }

  if (vectors_received == N) {
    ready_to_send = true;
    vectors_received = 0;
    CPU_time += worst_computation_time;
  }

}

void sendData() {
  //32 bytes per broadcast, each i-pair has 16 bytes
  uint8_t i = 0;
  while (i < N) {
    Wire.beginTransmission(0);
    I2C_writeAnything(positions[i]);  //12 bytes
    I2C_writeAnything(masses[i]); //4 bytes
    I2C_writeAnything(positions[i + 1]);  //12bytes
    I2C_writeAnything(masses[i + 1]); //4 bytes
    Wire.endTransmission();
    delayMicroseconds(i2c_PROPAGATION_DELAY);

    //    Serial.print("Sending: ");
    //    Serial.print(masses[i]);
    //    Serial.print(" kg, ");
    //    printVector(positions[i]);
    //    Serial.print("Sending: ");
    //    Serial.print(masses[i + 1]);
    //    Serial.print(" kg, ");
    //    printVector(positions[i + 1]);
    i += 2;
  }
  //Serial.println();
  ready_to_send = false;
}

void Signal() {
  altSerial.write('*');
}
