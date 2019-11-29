void printVector(Vector v) {
  Serial.print("{ ");
  Serial.print(v.x);
  Serial.print(" ");
  Serial.print(v.y);
  Serial.print(" ");
  Serial.print(v.z);
  Serial.println(" }");
}

void printSystem() {
  Serial.println();
  Serial.print("\tCycles: ");
  Serial.println(cycles);
  Serial.println();
  for (uint8_t i = 0; i < N; i++) {
    Serial.print("Body ");
    Serial.print(i + 1);
    Serial.print(" : ");
    Serial.print(masses[i]);
    Serial.print(" kg, ");
    printVector(positions[i]);
  }
  Serial.println();
}

void initiateSystem() {
  Serial.println("\tInitial System\n");
  int r;
  Vector pos_seed = {0.0, 0.0, 0.0};
  for (uint8_t i = 0; i < N; i++) {
    r = random(0, 2);
    switch (r) {      //increment distance for next body position
      case 0: pos_seed.x += 10; pos_seed.y += 5; break;
      case 1: pos_seed.y += 10; pos_seed.z += 5; break;
      case 2: pos_seed.z += 10; pos_seed.x += 5; break;
    }
    masses[i] = random(1, 10000);
    positions[i] = pos_seed;
    Serial.print("Body ");
    Serial.print(i + 1);
    Serial.print(" : ");
    Serial.print(masses[i]);
    Serial.print(" kg, ");
    printVector(positions[i]);
  }
  Serial.println();
}
