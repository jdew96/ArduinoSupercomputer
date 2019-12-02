#define HW_SERIAL_BAUD 115200
#define N 8 //matrix dimension(s)
#define X 1 //columns per device

unsigned long startTime;
unsigned long endTime;
//unsigned long receivedTime;
unsigned long CPU_time = 0;


float rando();
void multiplyMatrices();
void printMatrix();
void initiateMatrices();


float A[N][N];
float B[N][N];
float C[N][N];



void setup() {
  //initialize communication links
  Serial.begin(HW_SERIAL_BAUD);
  initiateMatrices();
  startTime = millis();
  multiplyMatrices();
  endTime = millis();
  printMatrix();
  Serial.print(F("CPU time: "));
  Serial.println(endTime - startTime);
}

void loop() {
}

/*int freeRam () { //should return free DRAM
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
  }*/
