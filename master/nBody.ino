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
uint8_t bodies = devices;
volatile body recieved; //is volatile necessary?


void setupNBody() {
  initiateSystem();
  Wire.onReceive(receiveBody);
  Serial.println("n-bodies initialized.");
}

void initiateSystem() {
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
    Serial.println("you gotta count the devices first."); //debug

  for (uint8_t i = 0; i < bodies; i++) {
    body new_body;
    new_body.mass = masses[i];
    new_body.pos = positions[i];
    new_body.vel = velocities[i];

    Serial.flush();
    Serial.println("sending");
    Serial.print("mass: :"); Serial.println(new_body.mass); //unnecessary
    Serial.print("position:"); Serial.print(new_body.pos.x);
    Serial.print(",");  Serial.print(new_body.pos.y);
    Serial.print(",");  Serial.println(new_body.pos.z);
    Serial.print("velocity:"); Serial.print(new_body.vel.x);
    Serial.print(",");  Serial.print(new_body.vel.y);
    Serial.print(",");  Serial.println(new_body.vel.z); Serial.println();
    Serial.flush();

    Wire.beginTransmission(i2c_ADDRESS_MIN);
    Wire.write((byte *) &new_body, sizeof new_body);
    Wire.endTransmission();
    delay(i2c_PROPAGATION_DELAY); //i guess this helps with consistency
  }
}

void receiveBody(uint8_t howMany) {
  if (howMany < sizeof recieved)
    return;

  // read into structure
  byte * b = (byte *) &recieved;
  for (byte i = 0; i < sizeof recieved; i++)
    *b++ = Wire.read();

  Serial.flush();
  Serial.println("recieved");
  Serial.println("body:");
  Serial.print("mass: :"); Serial.println(recieved.mass); //unnecessary
  Serial.print("position:"); Serial.print(recieved.pos.x);
  Serial.print(",");  Serial.print(recieved.pos.y);
  Serial.print(",");  Serial.println(recieved.pos.z);
  Serial.print("velocity:"); Serial.print(recieved.vel.x);
  Serial.print(",");  Serial.print(recieved.vel.y);
  Serial.print(",");  Serial.println(recieved.vel.z); Serial.println();
  Serial.flush();
}
