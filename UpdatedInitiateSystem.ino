void initiateSystem(){
  //things that should be hardcoded//////////////////////////
  GravConstant = 0.00000000006673;
  //i have no idea how lenient or constrained these limits are
  int mass_min =  1;    //0.01
  int mass_max =  101;  //1.00
  int pos_min =   -101; //-1.00
  int pos_max =   101;  //1.00
  int vel_min =   -101; //-1.00
  int vel_max =   101;  //1.00
  float decimal_resolution = 100.0;
  /////////////////////////////////////////////////////////////

  //for debug only, normally calculated during setup()
  int bodies = 18; 
  
  masses = (double*)malloc(bodies*sizeof(double));
  positions = (vector*)malloc(bodies*sizeof(vector));
  velocities = (vector*)malloc(bodies*sizeof(vector));
  accelerations = (vector*)malloc(bodies*sizeof(vector));

  Serial.print("Enter time steps: ");
  int time_steps = Serial.parseInt();
  if (Serial.read() == '\n')
  Serial.print("Performing n-body calculation on "); Serial.print(bodies); Serial.println(" bodies with pseudorandom positions and velocities.");  

  for (int i = 0; i < bodies; i++) {
    masses[i] = random(mass_min, mass_max) / decimal_resolution; 
    
    positions[i].x = random(pos_min, pos_max) / decimal_resolution; 
    positions[i].y = random(pos_min, pos_max) / decimal_resolution;
    positions[i].z = random(pos_min, pos_max) / decimal_resolution;
    
    velocities[i].x = random(vel_min, vel_max) / decimal_resolution;
    velocities[i].y = random(vel_min, vel_max) / decimal_resolution;
    velocities[i].z = random(vel_min, vel_max) / decimal_resolution;
  }
}
