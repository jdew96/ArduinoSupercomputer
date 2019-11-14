typedef struct {
  float x, y, z;
} vector;

typedef struct {
  float mass;
  vector pos;
  vector vel;
} body;

volatile body recieved; //volatile necessary?

void setupNBody() {
  Wire.onReceive(receiveBody);
  Serial.println("n-body mode initialized.");
}

void sendBody() {
  results_processed--;
  body created;

  created.mass = recieved.mass;
  created.pos.x = recieved.pos.x;
  created.pos.y = recieved.pos.y;
  created.pos.z = recieved.pos.z;
  created.vel.x = recieved.vel.x;
  created.vel.y = recieved.vel.y;
  created.vel.z = recieved.vel.z;
  created.pos.x *= 2;
  created.pos.y *= 2;
  created.pos.z *= 2;
  created.vel.x *= 2;
  created.vel.y *= 2;
  created.vel.z *= 2;

  //displays whats being sent on net
  Serial.flush();
  Serial.println("sending");
  Serial.print("mass:"); Serial.println(created.mass); //unnecessary
  Serial.print("position:"); Serial.print(created.pos.x);
  Serial.print(",");  Serial.print(created.pos.y);
  Serial.print(",");  Serial.println(created.pos.z);
  Serial.print("velocity:"); Serial.print(created.vel.x);
  Serial.print(",");  Serial.print(created.vel.y);
  Serial.print(",");  Serial.println(created.vel.z); Serial.println();

  //transmitting via i2c
  Wire.beginTransmission(i2c_MASTER_ADDRESS);
  Wire.write((byte *)&created, sizeof created);
  Wire.endTransmission();
  delay(i2c_PROPAGATION_DELAY);
}

void receiveBody(uint8_t howMany) {
  results_processed++;

  if ((uint8_t)howMany < sizeof recieved)
    return;

  // read into structure
  byte * p = (byte *) &recieved;
  for (byte i = 0; i < sizeof recieved; i++)
    *p++ = Wire.read();

  Serial.println("recieved");
  Serial.println("body:");
  Serial.print("mass: :"); Serial.println(recieved.mass); //unnecessary
  Serial.print("position:"); Serial.print(recieved.pos.x);
  Serial.print(",");  Serial.print(recieved.pos.y);
  Serial.print(",");  Serial.println(recieved.pos.z);
  Serial.print("velocity:"); Serial.print(recieved.vel.x);
  Serial.print(",");  Serial.print(recieved.vel.y);
  Serial.print(",");  Serial.println(recieved.vel.z); Serial.println();
}
