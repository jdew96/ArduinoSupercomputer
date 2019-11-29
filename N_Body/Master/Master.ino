#include <Wire.h>
#include <AltSoftSerial.h>
#include <I2C_Anything.h>
#define i2c_MASTER_ADDRESS 8
#define i2c_ADDRESS_MIN 9
#define i2c_ADDRESS_MAX 119
#define i2c_PROPAGATION_DELAY 10 //micro seconds
#define HW_SERIAL_BAUD 115200
#define SW_SERIAL_BAUD 9600
#define N 36 //number of bodies
#define X 12 //bodies per device
#define CYCLES 10 //cycles to simulate

typedef struct {
  float x, y, z;
} Vector;

void assignAddresses();
uint8_t countDevices();
void initiateSystem();
void printSystem();
void printVector(Vector v);
void receiveData(uint8_t size);
void sendData();
void Signal();


AltSoftSerial altSerial;
bool ready_to_send = false;
bool end_of_simulation = false;
uint8_t vectors_received = 0;

unsigned long startTime;
unsigned long endTime;
unsigned long receivedTime;
unsigned long CPU_time = 0;
unsigned long single_computation_time; //ms
unsigned long worst_computation_time = 0;


Vector positions[N];
float masses[N];
unsigned long int cycles = 0;



void setup() {
  //initialize communication links
  Serial.begin(HW_SERIAL_BAUD);
  Wire.begin(i2c_MASTER_ADDRESS);
  Wire.onReceive(receiveData);
  altSerial.begin(SW_SERIAL_BAUD);

  //initialize devices
  assignAddresses();
  delay(i2c_ADDRESS_MAX);
  //number of slaves should equal total bodies / bodies per slave, N should be even for most optimal package transfer
  if (countDevices() == (N / X) && N % 2 == 0) {
    Serial.println("Devices initiated!\n");
    initiateSystem();
    ready_to_send = true;
  }
  startTime = millis();
}

void loop() {

  if (ready_to_send && !end_of_simulation) {
    sendData();
    cycles++;
    Signal();
    Serial.print('.');
  }

  if (cycles == CYCLES && !end_of_simulation) {
    endTime = millis();
    printSystem();
    end_of_simulation = true;
    Serial.print("Total time: ");
    Serial.println(endTime - startTime);
    Serial.print("CPU time: ");
    Serial.println(CPU_time);
  }


}
