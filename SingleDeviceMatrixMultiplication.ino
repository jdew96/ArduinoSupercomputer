#define HW_SERIAL_BAUD 115200
uint8_t n = 2; // width/height of array

int **A, **B; // Create pointers for arrays which hold dummy data for multiplication
float **C; // Create pointers for array which hold dummy data for multiplication

void initiateData() {
  Serial.print("Array A has values: \n");

  A = malloc(n * sizeof(int*)); // Allocating space in first dimension of array to store floats
  for (uint8_t i = 0; i < n; i++) {
    A[i] = malloc(n * sizeof(int*)); // Allocating space for second dimension to store floats
    for (uint8_t j = 0; j < n; j++) {
      // Calculate decimal part randomly
      int decNum = random(0, 99);
      float decValue = decNum / 100.0;

      // Calculate integer part of numbers randomly
      int intValue = random(1, 100);

      float finalVal = intValue + decValue;
      A[i][j] = intValue;

      Serial.print(A[i][j]);
      Serial.print(' ');
    }
    Serial.print('\n');
  }

  B = malloc(n * sizeof(int*)); // Allocating space in first dimension of array to store floats
  Serial.print("Array B has values: \n");
  for (uint8_t i = 0; i < n; i++) {
    B[i] = malloc(n * sizeof(int*)); // Allocating space for second dimension to store floats
    for (uint8_t j = 0; j < n; j++) {
      // Calculate decimal part randomly
      int decNum = random(0, 99);
      float decValue = decNum / 100.0;

      // Calculate integer part of numbers randomly
      int intValue = random(1, 100);

      float finalVal = intValue + decValue;
      B[i][j] = intValue;

      Serial.print(B[i][j]);
      Serial.print(' ');
    }
    Serial.print('\n');
  }

  Serial.print('\n');
}

// Perform Matrix multiplication
void MuliplyMatrices() {
  Serial.print("The result of multiplication is as follows: \n");
  C = malloc(n * sizeof(float*)); // Allocating space in first dimension of array to store floats
  //float C[4][4];

  // Iterate through rows/columns
  // Result is sum of (row * column)
  for (uint8_t i = 0; i < n; i++) {
    C[i] = malloc(n * sizeof(float*)); // Allocating space for second dimension to store floats
    for (uint8_t j = 0; j < n; j++) {
      C[i][j] = 0.0;

      for (uint8_t k = 0; k < n; k++) {
        C[i][j] += A[i][k] * B[k][j];
      }
      Serial.print(C[i][j]);
      Serial.print(' ');
    }
    Serial.print('\n');
  }

}

void setup() {
  // put your setup code here, to run once:
  randomSeed(analogRead(A0));
  Serial.begin(HW_SERIAL_BAUD);
  initiateData();
  MuliplyMatrices();
}

void loop() {
  // put your main code here, to run repeatedly:

}
