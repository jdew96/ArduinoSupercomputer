#define decimal_resolution 100.00
#define mass_min 1       // 00.01
#define mass_max 101     // 01.00
#define pos_min -101     //-01.00
#define pos_max 101      // 01.00
#define vel_min -101     //-01.00
#define vel_max 101      // 01.00

typedef struct {
  float x, y, z;
} vector;

typedef struct {
  float mass;
  vector pos;
  vector vel;
} body;

float *masses;
vector *positions;
vector *velocities;
vector *accelerations;
uint8_t bodies;
volatile body received; //is volatile necessary?

void setupNBody() {
  initiateSystem();
  Serial.print(bodies); Serial.println(" bodies initialized.");
  Wire.onReceive(receiveBody);
}

void initiateSystem() {
  bodies = devices;
  masses = (float*)malloc(bodies * sizeof(float));
  positions = (vector*)malloc(bodies * sizeof(vector));
  velocities = (vector*)malloc(bodies * sizeof(vector));
  accelerations = (vector*)malloc(bodies * sizeof(vector));

  for (uint8_t i = 0; i < bodies; i++) {
    masses[i] = random(mass_min, mass_max) / decimal_resolution;
    positions[i].x = random(pos_min, pos_max) / decimal_resolution;
    positions[i].y = random(pos_min, pos_max) / decimal_resolution;
    positions[i].z = random(pos_min, pos_max) / decimal_resolution;
    velocities[i].x = random(vel_min, vel_max) / decimal_resolution;
    velocities[i].y = random(vel_min, vel_max) / decimal_resolution;
    velocities[i].z = random(vel_min, vel_max) / decimal_resolution;
  }
}

void sendBodies() {
  if (bodies <= 0) //debug
    Serial.println("Device list empty. Did you assign addresses?"); //debug

  for (uint8_t i = 0; i < bodies; i++) {
    body new_body;
    new_body.mass = masses[i];
    new_body.pos = positions[i];
    new_body.vel = velocities[i];

    Serial.println("sending");
    Serial.print("mass: :"); Serial.println(new_body.mass); //unnecessary
    Serial.print("position:"); Serial.print(new_body.pos.x);
    Serial.print(",");  Serial.print(new_body.pos.y);
    Serial.print(",");  Serial.println(new_body.pos.z);
    Serial.print("velocity:"); Serial.print(new_body.vel.x);
    Serial.print(",");  Serial.print(new_body.vel.y);
    Serial.print(",");  Serial.println(new_body.vel.z); Serial.println();

    Wire.beginTransmission(i2c_ADDRESS_MIN);
    Wire.write((byte *) &new_body, sizeof new_body);
    Wire.endTransmission();
    delay(i2c_PROPAGATION_DELAY); //i guess this helps with consistency
  }
}

void receiveBody(int howMany) {
  if ((uint8_t)howMany < sizeof received)
    return;

  // read into structure
  byte * b = (byte *) &received;
  for (byte i = 0; i < sizeof received; i++)
    *b++ = Wire.read();

  Serial.println("received");
  Serial.println("body:");
  Serial.print("mass: :"); Serial.println(received.mass); //unnecessary
  Serial.print("position:"); Serial.print(received.pos.x);
  Serial.print(",");  Serial.print(received.pos.y);
  Serial.print(",");  Serial.println(received.pos.z);
  Serial.print("velocity:"); Serial.print(received.vel.x);
  Serial.print(",");  Serial.print(received.vel.y);
  Serial.print(",");  Serial.println(received.vel.z); Serial.println();
}
