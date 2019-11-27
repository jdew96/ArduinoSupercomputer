//rename file to //Serial
void informNeighbor() {
  //  altSerial.begin(SW_//Serial_BAUD);
  while (a_transmission_complete == false)
    altSerial.write('A');
  //  altSerial.end(); //a likely futile attempt to free more dynamic memory during runtime
//    //Serial.println("informed slave.");
}

void assignAddresses() {
  unsigned char data = i2c_ADDRESS_MIN; //i2c useable adddress range: 8 - 119

  //Serial.print(F("Addressing i2c devices starting with 0x"));
  //Serial.print(data, HEX);
  //Serial.println(F("..."));

  altSerial.write(data);
  delay (i2c_PROPAGATION_DELAY); //make sure it finished sending
  //  altSerial.end();
}
