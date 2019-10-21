void assignAddresses() {
  unsigned char data = i2c_ADDRESS_MIN; //i2c useable adddress range: 8 - 119

  Serial.print("Addressing i2c devices starting with 0x");
  Serial.print(data, HEX);
  Serial.println("...");

  altSerial.write(data);
}
