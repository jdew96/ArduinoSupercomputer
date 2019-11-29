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

typedef struct {
  float x, y, z;
} Vector;

void assignAddresses();
void receiveAddress();
void receiveData(uint8_t size);
void sendData();
void Signal();
void Wait();

void printVector(Vector v);
float magnitude(Vector v);
float square(float x);
Vector add_vector(Vector a, Vector b);
Vector subtract_vector(Vector a, Vector b);
Vector scale_vector(Vector v, float factor);
void compute_accelerations();
void compute_velocities();
void compute_positions();

AltSoftSerial altSerial;
uint8_t i2c_LOCAL_ADDRESS = 0;

float GRAV_CONST = 0.1;
float EPSILON = 10;

bool ready_to_send = false;
uint8_t packets = 0; //itearator for receiving packets
uint8_t slave_index, first, last;

Vector positions[N];
float masses[N];
Vector local_accelerations[X] = {0, 0, 0};
Vector local_velocities[X] = {0, 0, 0};
Vector local_positions[X] = {0, 0, 0};

void setup() {
  //initialize communication
  Serial.begin(HW_SERIAL_BAUD);
  altSerial.begin(SW_SERIAL_BAUD);

  receiveAddress();
  slave_index = i2c_LOCAL_ADDRESS - i2c_ADDRESS_MIN; //index of device
  first = slave_index * X; // index of first body in local set
  last = first + X - 1; // index of last body in local set

  Wire.onReceive(receiveData);
}

void loop() {
  if (ready_to_send) {
    Wait();
    sendData();
    Signal();
  }
}
