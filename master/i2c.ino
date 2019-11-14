typedef struct {
  bool _operation;
  uint8_t _devices;
  uint8_t _workload_magnitude;
} opcode;

void broadcastOpcode(bool operation) {
  opcode new_opcode;
  new_opcode._operation = operation;
  new_opcode._devices = devices;
  new_opcode._workload_magnitude = WORKLOAD_MAGNITUDE;
  
  Wire.beginTransmission(0); //broadcast?
  Wire.write((byte *) &new_opcode, sizeof new_opcode);
  Wire.endTransmission();
  delay(i2c_PROPAGATION_DELAY); //i guess this helps with consistency
}
