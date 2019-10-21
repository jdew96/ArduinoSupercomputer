//interrupt pins: 0, 1, 2, 3, 7
//altSoftSerial pins: TX:9, RX:8
//i2c pins: SDA:A4, SCL:A5.

#include <Wire.h>
#include <AltSoftSerial.h>
#define i2c_MASTER_ADDRESS 8
#define i2c_ADDRESS_MIN 9
#define i2c_ADDRESS_MAX 119
#define i2c_PROPAGATION_DELAY 5 //this could be a bottleneck at some point
#define HW_SERIAL_BAUD 115200
#define SW_SERIAL_BAUD 9600

AltSoftSerial altSerial;
void commandPrompt();
void initiateSystem();
void sendBodies();
void receiveBody(uint8_t);
uint8_t i2c_LOCAL_ADDRESS = 0;//move

uint8_t results_processed = 0;
uint8_t workload_magnitude = 1;
uint8_t devices = 1; //it's all up to you, my man.


void setup() {
  Serial.begin(HW_SERIAL_BAUD);
  altSerial.begin(SW_SERIAL_BAUD);
  Serial.println("setup complete.");
  Wire.onReceive(recieveOpcode);
  receiveAddress();
}

void loop() {
while(results_processed !=0)
  sendBody();
}
