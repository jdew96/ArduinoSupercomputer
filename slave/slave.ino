//interrupt pins: 0, 1, 2, 3, 7
//altSoftSerial pins: TX:9, RX:8
//i2c pins: SDA:A4, SCL:A5.
#define M_SIZE 8


#include <Wire.h>
#include <AltSoftSerial.h>
#include <I2C_Anything.h>
#include <BasicLinearAlgebra.h>
#include <avr/pgmspace.h> //for progmem migration
#define i2c_MASTER_ADDRESS 8
#define i2c_ADDRESS_MIN 9
#define i2c_ADDRESS_MAX 119
#define i2c_PROPAGATION_DELAY 5 //this could be a bottleneck at some point
#define HW_SERIAL_BAUD 115200
#define SW_SERIAL_BAUD 9600

AltSoftSerial altSerial;
void commandPrompt();
void initiateSystem();
void sendBody();
void receiveBody(uint8_t);
void receiveOpcode(uint8_t);
void multiplyAndSendMatrices();

uint8_t i2c_LOCAL_ADDRESS = 0;
uint8_t results_processed = 0;
uint8_t workload_magnitude = 1;
uint8_t devices = 1;

uint8_t iter_x = 0, iter_y = 0;
bool done_receiving_a = false;
bool done_receiving_b = false;

void setup() {
  Serial.begin(HW_SERIAL_BAUD);
  altSerial.begin(SW_SERIAL_BAUD);
  //Wire.setClock(400000L); //full throttle for i2c, check for stability when enabling
  Wire.onReceive(receiveOpcode);
  Serial.println("setup complete.");
  receiveAddress();
}

void loop() {
  if (done_receiving_b == true) {
    iter_x = 0, iter_y = 0;
    done_receiving_b = false;
    Serial.println();
    Wire.onReceive(receiveMatrixA);
  }
  if (done_receiving_a == true) {
    iter_x = 0, iter_y = 0;
    done_receiving_a = false;
    Wire.onReceive(receiveOpcode);
    multiplyAndSendMatrices();
  }
  //Serial.print('.');
}
