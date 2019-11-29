#define HW_SERIAL_BAUD 115200
#define N 36
#define CYCLES 10

//define a 3D vector
typedef struct {
  float x, y, z;
} Vector;

float GRAV_CONST = 0.1;
float EPSILON = 10;
uint8_t TIME_STEP = 1;

Vector accelerations[N] = {0, 0, 0};
Vector velocities[N] = {0, 0, 0};
Vector positions[N];
float masses[N];
unsigned long cycles = 0;
bool end_of_simulation = false;
unsigned long startTime;
unsigned long endTime;
unsigned long totalTime;
unsigned long startms;
unsigned long endms;

//helper functions to perform vector calculations
Vector add_vector(Vector a, Vector b);
Vector subtract_vector(Vector a, Vector b);
Vector scale_vector(Vector v, float factor);
float magnitude(Vector v);
float square(float x);
void initiateSystem();
void printSystem();
void printVector(Vector v);

//functions to calculate equations of motions
void compute_accelerations();
void compute_velocities();
void compute_posistions();

//simulate N-Body system for one timestep
void simulate(int cycles);

void setup() {
  Serial.begin(HW_SERIAL_BAUD);
  initiateSystem();
  startms = millis();
}

void loop() {
  if (cycles < CYCLES && !end_of_simulation) {
    startTime = micros();
    compute_accelerations();
    compute_velocities();
    compute_posistions();
    endTime = micros();
    totalTime = endTime - startTime;
    cycles++;
    //delay(OVERHEAD);
    Serial.print('.');
  }
  if (cycles == CYCLES && !end_of_simulation) {
    endms = millis();
    Serial.println();
    printSystem();
    Serial.print("CPU Time: ");
    Serial.print(endms - startms);
    Serial.println();
    end_of_simulation = true;
  }
}
