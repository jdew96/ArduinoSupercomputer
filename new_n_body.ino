
#define decimal_resolution 100.00
#define mass_min 1       // 00.01
#define mass_max 101     // 01.00
#define pos_min -101     //-01.00
#define pos_max 101      // 01.00
#define vel_min -101     //-01.00
#define vel_max 101      // 01.00




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  initiateSystem();

}

void loop() {
  // put your main code here, to run repeatedly:

}


typedef struct {
  float x, y, z;
} vector;

typedef struct {
  float mass;
  vector pos;
  vector vel;
} body;

int devices;
float *masses;
vector *positions;
vector *velocities;
vector *accelerations;
uint8_t bodies;
volatile body received; //is volatile necessary?
double GravConstant = 0.01;
int timeSteps = 10;



void initiateSystem() {

  devices = 2;
  masses = (float*)malloc(bodies * sizeof(float));
  positions = (vector*)malloc(bodies * sizeof(vector));
  velocities = (vector*)malloc(bodies * sizeof(vector));
  accelerations = (vector*)malloc(bodies * sizeof(vector));

  for (uint8_t i = 0; i < bodies; i++) {
    masses[i] = 5;
    positions[i].x = 5;
    positions[i].y = 5;
    positions[i].z = 5;
    velocities[i].x = 5;
    velocities[i].y = 5;
    velocities[i].z = 5;

    // positions[i].x = 3;
    Serial.println(positions[i].x);
    //    positions[i].y = 3 / decimal_resolution;
    //    positions[i].z = 3 / decimal_resolution;
    //    velocities[i].x = 3 / decimal_resolution;
    //    velocities[i].y = 3 / decimal_resolution;
    //    velocities[i].z = 3 / decimal_resolution;
  }

  for (int i = 0; i < devices; i++)
  {
    body new_body;
    new_body.mass = masses[i];
    new_body.pos = positions[i];
    new_body.vel = velocities[i];
  }
  //main
  int i;
  int j;
  Serial.print("Body   :     x              y           /    z           |           vx              vy              vz   ");
  for (i = 0; i < timeSteps; i++) {
    Serial.print("\nCycle"); Serial.println(i + 1);
    //simulate();
    for (j = 0; j < devices; j++) {
      //printf("Body %d : %lf\t%f\t%lf\t|\t%lf\t%lf\t%lf\n",j+1,positions[j].x,positions[j].y,positions[j].z,velocities[j].x,velocities[j].y,velocities[j].z);
    
        body new_body;
        new_body.mass = masses[j];
        new_body.pos = positions[j];
        new_body.vel = velocities[j];

        Serial.print("Body");
        Serial.print(j + 1);
        Serial.print("\t");
        Serial.print(new_body.pos.x);
        Serial.print("\t");
        Serial.print(new_body.pos.y);
        Serial.print("\t");
        Serial.print(new_body.pos.z);
        Serial.print("\t");
        Serial.print(new_body.vel.x);
        Serial.print("\t");
        Serial.print(new_body.vel.y);
        Serial.print("\t");
        Serial.print(new_body.vel.z);
        Serial.print("\n");
      
    }

  }
}

void simulate() {
  //computeAccelerations();
  //computePositions();
  //computeVelocities();
  //resolveCollisions();
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

    // Wire.beginTransmission(i2c_ADDRESS_MIN);
    // Wire.write((byte *) &new_body, sizeof new_body);
    //Wire.endTransmission();
    //delay(i2c_PROPAGATION_DELAY); //i guess this helps with consistency
  }
}
