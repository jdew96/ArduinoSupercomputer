//altSoftSerial pins: TX:9, RX:8
//i2c pins: SDA:A4, SCL:A5.


#include <Wire.h>
#include <AltSoftSerial.h>
#include <I2C_Anything.h>
//#include <BasicLinearAlgebra.h>
#define i2c_MASTER_ADDRESS 8
#define i2c_ADDRESS_MIN 9
#define i2c_ADDRESS_MAX 119
#define i2c_PROPAGATION_DELAY 10 //micro seconds
#define HW_SERIAL_BAUD 115200
#define SW_SERIAL_BAUD 31250
#define i2c_CLOCK 400000L
#define N 8 //matrix dimension(s)
#define X 1 //columns per device
uint8_t datatype_size = BUFFER_LENGTH/sizeof(float); //bytes
uint8_t i2c_LOCAL_ADDRESS = 0;


AltSoftSerial altSerial;
uint8_t iter_x = 0, iter_y = 0;
bool done_receiving_a = false;
bool done_receiving_b = false;
unsigned long CPU_time = 0;


float A[N][X];
float B[N][N];
float C[N][X];


void setup() {
  //initialize communication
  Serial.begin(HW_SERIAL_BAUD);
  altSerial.begin(SW_SERIAL_BAUD);
  receiveAddress();
  Wire.onReceive(receiveMatrixB);
}

void loop() {
  if (done_receiving_b == true) {
    iter_x = 0, iter_y = 0;
    done_receiving_b = false;
    Serial.println();
    Wire.onReceive(receiveMatrixA);
  }
  if (done_receiving_a == true) {
    done_receiving_a = false;
    iter_x = 0, iter_y = 0;
    multiplyMatrices();
    Wait();
    sendData();
    Signal();
    Wire.onRequest(giveCPUtime);
  }
}

/*int freeRam () { //should return free DRAM
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}*/
