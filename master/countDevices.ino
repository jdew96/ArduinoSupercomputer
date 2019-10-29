void countDevices() {
  devices = 0;
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

    delay (i2c_PROPAGATION_DELAY); //give devices time to recover
  }

  Serial.println ("Done.");
  Serial.print ("Found ");
  Serial.print (devices);
  Serial.println (" device(s).");
}
