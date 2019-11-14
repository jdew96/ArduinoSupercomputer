void assignAddresses() {
  if (i2c_LOCAL_ADDRESS != 0) {
    unsigned char data = i2c_LOCAL_ADDRESS + 1; //i2c useable adddress range: 8 - 119

    Serial.print(F("Addressed next device i2c with 0x"));
    Serial.println(data, HEX);

    altSerial.write(data);
  }
}

void receiveAddress() {
  Serial.println(F("Waiting on i2c address..."));
  unsigned char input;
  while (i2c_LOCAL_ADDRESS == 0) {
    input = altSerial.read();
    if (input >= i2c_ADDRESS_MIN && input <= i2c_ADDRESS_MAX) {
      Serial.print(F("i2c address set to 0x"));
      Serial.println(input, HEX);
      i2c_LOCAL_ADDRESS = input;
      Wire.begin(i2c_LOCAL_ADDRESS);
      TWAR = (i2c_LOCAL_ADDRESS << 1) | 1;  // enable broadcasts to be received
      altSerial.end(); //a likely futile attempt to free more dynamic memory during runtime
    }
    else {
      if (input != 255) {
        Serial.print(F("i2c address 0x"));
        Serial.print(input, HEX);
        Serial.println(F(" invalid."));
      }
    }
  }
  if (i2c_LOCAL_ADDRESS < i2c_ADDRESS_MAX)
    assignAddresses();
}
