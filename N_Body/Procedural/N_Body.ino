//-------------------------------------------------------------------------------------------------------------------


Vector add_vector(Vector a, Vector b) {
  Vector c = {a.x + b.x, a.y + b.y, a.z + b.z};
  return c;
}
Vector subtract_vector(Vector a, Vector b) {
  Vector c = {a.x - b.x, a.y - b.y, a.z - b.z};
  return c;
}
Vector scale_vector(Vector v, float factor) {
  Vector c = {factor * v.x, factor * v.y, factor * v.z};
  return c;
}
float magnitude(Vector v) {
  return sqrt(v.x * v.x + v.y * v.y + v.z * v.z); //Sqrt(x^2 + y^2 + z^2)
}
float square(float x) {
  return x * x;
}


//-------------------------------------------------------------------------------------------------------------------


void compute_accelerations() {
  //calcucalte acceleration for each body
  int i, j;
  for (i = 0; i < N; i++) {
    //initialize sum of forces acting on Body[i]
    Vector sum_of_forces = {0.0, 0.0, 0.0};
    for (j = 0; j < N; j++) {
      if (i != j) {
        Vector radius_ij = subtract_vector(positions[j], positions[i]);
        /* M / (|r|^2 + e^2)^(3/2) */
        float scale_factor = masses[j] / (sqrt(pow(square(magnitude(radius_ij)) + square(EPSILON), 3)));
        Vector force_ij = scale_vector(radius_ij, scale_factor);
        sum_of_forces = add_vector(sum_of_forces, force_ij);
      }
    }
    //calculate acceleration on Body[i] by scaling the sum of forces acting on Body[i] with gravitational constant
    accelerations[i] = scale_vector(sum_of_forces, GRAV_CONST);
  }
}
//v = v0 + at
void compute_velocities() {
  int i;
  //calculate new velocity for each body after time step
  for (i = 0; i < N; i++) {
    //final velocity = initial velocity + acceleration * time
    velocities[i] = add_vector(velocities[i], scale_vector(accelerations[i], TIME_STEP));
  }
}
//x = x0 + v0 + 1/2 at^2
void compute_posistions() {
  int i;
  //calculate new positions for each body after time step
  for (i = 0; i < N; i++) {
    //displacement = velocity + 1/2 * acceleration * time^2
    Vector displacement = add_vector(velocities[i], scale_vector(accelerations[i], 0.5 * TIME_STEP * TIME_STEP));
    //final position = initial position + displacement
    positions[i] = add_vector(positions[i], displacement);
  }
}


//-------------------------------------------------------------------------------------------------------------------


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
