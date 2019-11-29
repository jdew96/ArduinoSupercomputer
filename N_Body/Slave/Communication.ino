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

void receiveData(uint8_t size) {
  //  Serial.print("Interrupt... size: ");
  //  Serial.println(size);
  //recievs a packet from broadcast size of 2 vectors and 2 floats
  if (size >= 2 * (sizeof(Vector) + sizeof(float))) {

    //Serial.println("recieving...");
    I2C_readAnything(positions[packets]);
    I2C_readAnything(masses[packets]);

    //    Serial.print("recieved: ");
    //    Serial.print(masses[packets]);
    //    Serial.print(" kg, ");
    //    printVector(positions[packets]);
    packets++;

    I2C_readAnything(positions[packets]);
    I2C_readAnything(masses[packets]);

    //    Serial.print("recieved: ");
    //    Serial.print(masses[packets]);
    //    Serial.print(" kg, ");
    //    printVector(positions[packets]);
    packets++;
  }

  if (packets == N) {
    ready_to_send = true;
    packets = 0;
  }
}

void sendData() {
  unsigned long start;
  unsigned long finish;
  start = millis();
  compute_accelerations();
  compute_velocities();
  compute_positions();
  finish = millis();
  uint8_t i = 0;
  while (i < X) {
    Wire.beginTransmission(i2c_MASTER_ADDRESS);

    I2C_writeAnything(local_positions[i]);
    //Serial.print("Sending: ");
    //printVector(local_positions[i]);
    i++;

    I2C_writeAnything(local_positions[i]);
    //Serial.print("Sending: ");
    //printVector(local_positions[i]);
    i++;

    I2C_writeAnything(finish - start);
    //Serial.print("Sending: ");
    //Serial.println (finish - start);

    Wire.endTransmission();
    delayMicroseconds(i2c_PROPAGATION_DELAY);
  }


}

void Signal() {
  //Serial.println("Signaling...");
  altSerial.write('*');
}

void Wait() {
  char c = '0';
  //Serial.println("Listening...");
  while (c != '*') {
    c = altSerial.read();
  }
  //Serial.println(c);
}
