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

void compute_accelerations() {

  for (uint8_t i = first; i <= last; i++) {
    Vector sum_of_forces = {0.0, 0.0, 0.0};
    for (uint8_t j = 0; j < N; j++) {
      if (i != j ) {
        //initialize sum of forces acting on Body[i]
        Vector radius_ij = subtract_vector(positions[j], positions[i]);
        float scale_factor = masses[j]/ (sqrt(pow(square(magnitude(radius_ij)) + square(EPSILON),3)));
        Vector force_ij = scale_vector(radius_ij, scale_factor);
        sum_of_forces = add_vector(sum_of_forces,force_ij);
      }
    }
    //calculate acceleration on Body[i] by scaling the sum of forces acting on Body[i] with gravitational constant
    local_accelerations[i - first] = scale_vector(sum_of_forces,GRAV_CONST);
  }
}

//v = v0 + at
void compute_velocities(){
  for(uint8_t i = 0; i < X; i++){
    //final velocity = initial velocity + acceleration * time
    local_velocities[i] = add_vector(local_velocities[i], local_accelerations[i]);
  }
}

//x = x0 + v0 + 1/2 at^2
void compute_positions(){
  for (uint8_t i = 0; i < X; i++){
    //displacement = velocity + 1/2 * acceleration * time^2
    Vector displacement = add_vector(local_velocities[i], scale_vector(local_accelerations[i], 0.5));
    //final position = initial position + displacement
    local_positions[i] = add_vector(local_positions[i], displacement);
  }
}


void printVector(Vector v) {
  Serial.print("{ ");
  Serial.print(v.x);
  Serial.print(" ");
  Serial.print(v.y);
  Serial.print(" ");
  Serial.print(v.z);
  Serial.println(" }");
}
