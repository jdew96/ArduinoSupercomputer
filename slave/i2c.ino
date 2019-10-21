#define MATRIX 0
#define NBODY 1


typedef struct {
  bool _operation;
  uint8_t _devices;
  uint8_t _workload_magnitude;
} opcode;

void recieveOpcode(uint8_t howMany) {
  opcode received; //volatile opcode received;
  if ((uint8_t)howMany < sizeof received)
    return;

  // read into structure
  byte * b = (byte *) &received;
  for (byte i = 0; i < sizeof received; i++)
    *b++ = Wire.read();

  Serial.println("opcode received:");

  devices = received._devices;
  workload_magnitude = received._workload_magnitude;
  Serial.print("devices:"); Serial.print(devices);
  Serial.print(", workload magnitude:"); Serial.print(workload_magnitude);
  Serial.print(", operation:");
  if (received._operation == MATRIX) {
    Serial.println("MATRIX");
    setupMatrices();
  }
  else if (received._operation == NBODY) {
    Serial.println("NBODY");
    setupNBody();
  }
}
