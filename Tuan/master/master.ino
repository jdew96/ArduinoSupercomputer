#include <Wire.h>
#include <AltSoftSerial.h>
//#define MATRIX 0
//#define NBODY 1
#define i2c_MASTER_ADDRESS 8
#define i2c_ADDRESS_MIN 9
#define i2c_ADDRESS_MAX 119
#define i2c_PROPAGATION_DELAY 5 //this could be a bottleneck at some point
#define HW_SERIAL_BAUD 115200
#define SW_SERIAL_BAUD 9600

AltSoftSerial altSerial;
uint8_t devices = 0;

void assignAddresses();
void countDevices();

void setup() {
  Serial.begin(HW_SERIAL_BAUD);
  altSerial.begin(SW_SERIAL_BAUD);
  Wire.begin(i2c_MASTER_ADDRESS);
}

void loop() {
  // put your main code here, to run repeatedly:

}





//----------------------------------------------------------------------------------
void assignAddresses() {
  unsigned char data = i2c_ADDRESS_MIN; //i2c useable adddress range: 8 - 119

  Serial.print("Addressing i2c devices starting with 0x");
  Serial.print(data, HEX);
  Serial.println("...");

  altSerial.write(data);
}

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
//----------------------------------------------------------------------------------
