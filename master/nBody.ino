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

//float masses[devices];
//float positions[3][devices];
//float velocities[3][devices];
//float accelerations[3][devices];

void setupNBody() {
  //Serial.println(" body initialized.");
  initiateSystem();
  //  Wire.onReceive(receiveAcceleration);
}

void initiateSystem() {
  //    mass = random(mass_min, mass_max) / decimal_resolution;
  //  pos.x = random(pos_min, pos_max) / decimal_resolution;
  //  pos.y = random(pos_min, pos_max) / decimal_resolution;
  //  pos.z = random(pos_min, pos_max) / decimal_resolution;
  //  vel.x = random(vel_min, vel_max) / decimal_resolution;
  //  vel.y = random(vel_min, vel_max) / decimal_resolution;
  //  vel.z = random(vel_min, vel_max) / decimal_resolution;
}
