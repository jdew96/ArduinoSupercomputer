//interrupt pins: 0, 1, 2, 3, 7
//altSoftSerial pins: TX:9, RX:8
//i2c pins: SDA:A4, SCL:A5.

#include <Wire.h>
#include <AltSoftSerial.h>
#define MATRIX 0
#define NBODY 1
#define i2c_MASTER_ADDRESS 8
#define i2c_ADDRESS_MIN 9
#define i2c_ADDRESS_MAX 119
#define i2c_PROPAGATION_DELAY 5 //this could be a bottleneck at some point
#define HW_SERIAL_BAUD 115200
#define SW_SERIAL_BAUD 9600
uint8_t WORKLOAD_MAGNITUDE = 1;
uint8_t devices = 0;

AltSoftSerial altSerial;
void commandPrompt();
void assignAddresses();
void countDevices();
void initiateSystem();
void sendBodies();
void receiveBody(int);
void setupNBody();
void broadcastOpcode(bool);
void setupMatrices();
void sendMatrices();
// ^^^some of these can probably be commented out

void setup() {
  Serial.begin(HW_SERIAL_BAUD);
  altSerial.begin(SW_SERIAL_BAUD);
  Wire.begin(i2c_MASTER_ADDRESS);

  Serial.println("setup complete.");
  Serial.println("Welcome to the shit show.\n");
  Serial.println("'A' to assign addresses.");
  //Serial.println("'C' to count devices.");
  Serial.println("'N' to initiate n-body sequence.");
  Serial.println("'M' to initiate dot-product sequence.");
  Serial.println("numbers 1-9 to change workload magnitude.");
}

void loop() {
  commandPrompt();
}

void commandPrompt() {
  boolean newData = false;
  char input;

  if (Serial.available()) {
    input = Serial.read();
    newData = true;
  }

  if (newData == true && input != 13 && input != 10) { //igonring CR and LF
    newData = false;
    switch (input) {
      case 'a':
      case 'A':
        assignAddresses();
        delay(i2c_PROPAGATION_DELAY * i2c_ADDRESS_MAX);
        countDevices();
        break;
      /* case 'c':
        case 'C':
         countDevices();
         break;*/
      case 'n':
      case 'N':
        if (devices <= 0) //debug
          Serial.println("Device list empty. Did you assign addresses?"); //debug
        else {
          setupNBody();
          broadcastOpcode(NBODY);
          sendBodies();
        }
        break;
      case 'm':
      case 'M':
        if (devices <= 0) //debug
          Serial.println("Device list empty. Did you assign addresses?"); //debug
        else {
          setupMatrices();
          broadcastOpcode(MATRIX);
          sendMatrices();
        }
        break;
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        changeMagnitude(input - '0');
        break;
      default:
        //i guess debug functions will run here
        break;
    }
  }
}

void changeMagnitude(uint8_t magnitude) {
  WORKLOAD_MAGNITUDE = magnitude;
  Serial.print("Workload magnitude changed to "); Serial.println(WORKLOAD_MAGNITUDE);
}
