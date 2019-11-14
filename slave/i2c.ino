#define MATRIX 0
#define NBODY 1


typedef struct {
  bool _operation;
  uint8_t _devices;
  uint8_t _workload_magnitude;
} opcode;

void receiveOpcode(uint8_t howMany) {
  opcode received;
  if ((uint8_t)howMany < sizeof received)
    return;

  // read into structure
  byte * b = (byte *) &received;
  for (byte i = 0; i < sizeof received; i++)
    *b++ = Wire.read();

  devices = received._devices;
  workload_magnitude = received._workload_magnitude;
  Serial.print(F("opcode received: { [devices:")); Serial.print(devices);
  Serial.print(F("][workload:")); Serial.print(workload_magnitude);
  Serial.print(F("][operation:"));
  if (received._operation == MATRIX) {
    Serial.println(F("MATRIX] }\n"));
    setupMatrices();
  }
  else if (received._operation == NBODY) {
    Serial.println(F("NBODY] }\n"));
    setupNBody();
  }
}
