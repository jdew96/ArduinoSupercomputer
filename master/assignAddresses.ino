void assignAddresses() {
  unsigned char data = i2c_ADDRESS_MIN; //i2c useable adddress range: 8 - 119

  Serial.print("Started addressing i2c at 0x");
  Serial.println(data, HEX);

  altSerial.write(data);
}
