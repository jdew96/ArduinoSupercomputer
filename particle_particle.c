#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
// #include<pthread.h>
// #include<sys/sem.h>
// #include<sys/types.h>
// #include<sys/ipc.h>
// #include<unistd.h>

// #define SEMKEY ((key_t) 400L) 
// #define NSEMS 2  //defines the number of semaphores to be created
#define GRAV_CONST 0.1
#define	TIME_STEP 1
#define EPSILON 10

//define a 3D vector 
typedef struct{
	float x, y, z;
} Vector;

//global variables
int N, cycles;
float* masses;
Vector* accelerations;
Vector* velocities;
Vector* positions;

//helper functions to perform vector calculations
Vector add_vector(Vector a, Vector b);
Vector subtract_vector(Vector a, Vector b);
Vector scale_vector(Vector v, float factor);
float magnitude(Vector v);
float square(float x);

//functions to calculate equations of motions
void compute_accelerations();
void compute_velocities();
void compute_posistions();
void compute_collisions();

//simulate N-Body system for one timestep
void simulate(int cycles);

int main(int argC,char* argV[]){
	//invalid arguments
	if (argC < 3){
		printf("Not enough arguments provided!\nArguments: <N bodies> <total cycles>");
		return 0;
	}
	else if (atoi(argV[1]) == 0 || atoi(argV[2]) == 0){
	 	printf("Arguments must be non-zero integer values!\nArguments: <N bodies> <total cycles>");
	 	return 0;
	}

	//initialize system
	N = atoi(argV[1]);
	cycles = atoi(argV[2]);
	masses = (float*)malloc(N*sizeof(float));
	positions = (Vector*)malloc(N*sizeof(Vector));
	velocities = (Vector*)malloc(N*sizeof(Vector));
	accelerations = (Vector*)malloc(N*sizeof(Vector));

	srand(time(NULL));
	Vector pos_seed = {0.0,0.0,0.0};	//seed positions of body with unique xyz coordinates
	Vector initial_velocity = {0.0,0.0,0.0}; //set initial velocity to zero
	int i, r;
	for(i = 0; i < N; i++){
		r = rand() % 3;
		switch (r){				//increment distance for next body position
			case 0: pos_seed.x += 10; pos_seed.y += 5; break;
			case 1: pos_seed.y += 10; pos_seed.z += 5; break;
			case 2: pos_seed.z += 10; pos_seed.x += 5; break;
		}
		positions[i] = pos_seed;	//seed initial positions (unique for each body)
		velocities[i] = initial_velocity;	
		masses[i] = rand() % 10000 + 1; // seed initial mass [1,10000]
		//printf("%f\t%f\t%f\n", positions[i].x,positions[i].y,positions[i].z);
	}
	simulate(cycles);
	return 0;
}


//-------------------------------------------------------------------------------------------------------------------


Vector add_vector(Vector a, Vector b){
	Vector c = {a.x + b.x, a.y + b.y, a.z + b.z};
	return c;
}
Vector subtract_vector(Vector a, Vector b){
	Vector c = {a.x - b.x, a.y - b.y, a.z - b.z};
	return c;
}
Vector scale_vector(Vector v, float factor){
	Vector c = {factor*v.x, factor*v.y, factor*v.z};
	return c;
}
float magnitude(Vector v){
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);	//Sqrt(x^2 + y^2 + z^2)
}
float square(float x){
	return x*x;
}


//-------------------------------------------------------------------------------------------------------------------


void compute_accelerations(){
	//calcucalte acceleration for each body
	int i,j;
	for(i = 0; i < N; i++){
		//initialize sum of forces acting on Body[i]
		Vector sum_of_forces = {0.0,0.0,0.0}; 
		for(j = 0; j < N; j++){
			if(i != j){
				Vector radius_ij = subtract_vector(positions[j], positions[i]);
				/* M / (|r|^2 + e^2)^(3/2) */
				float scale_factor = masses[j]/ (sqrt(pow(square(magnitude(radius_ij)) + square(EPSILON),3))); 
				Vector force_ij = scale_vector(radius_ij, scale_factor); 
				sum_of_forces = add_vector(sum_of_forces,force_ij);
			}
		}
		//calculate acceleration on Body[i] by scaling the sum of forces acting on Body[i] with gravitational constant
		accelerations[i] = scale_vector(sum_of_forces,GRAV_CONST);
	}
}
//v = v0 + at
void compute_velocities(){
	int i;
	//calculate new velocity for each body after time step
	for (i = 0; i < N; i++){
		//final velocity = initial velocity + acceleration * time
		velocities[i] = add_vector(velocities[i], scale_vector(accelerations[i],TIME_STEP));
	}
}
//x = x0 + v0 + 1/2 at^2
void compute_posistions(){
	int i;
	//calculate new positions for each body after time step
	for (i = 0; i < N; i++){
		//displacement = velocity + 1/2 * acceleration * time^2
		Vector displacement = add_vector(velocities[i], scale_vector(accelerations[i], 0.5*TIME_STEP*TIME_STEP));
		//final position = initial position + displacement
		positions[i] = add_vector(positions[i], displacement);
	}
}
void compute_collisions(){
	int i,j;
	for(i=0;i<N-1;i++)
		//check each body with every other body for collisions
		for(j=i+1;j<N;j++){
			//if there's a collision, simulate elastic collision
			if(positions[i].x==positions[j].x && positions[i].y==positions[j].y && positions[i].z==positions[j].z){
				Vector temp = velocities[i];
				velocities[i] = velocities[j];
				velocities[j] = temp;
			}
		}
}


//-------------------------------------------------------------------------------------------------------------------


void simulate(int cycles){
	int i, j;
	//print initial state
	printf("Body\t\tX\t\tY\t\tZ\n------------------------------------------------------------\n");
	printf("Cycle 0\n");
	for(j=0;j<N;j++)
		printf("Body %d: \t%f\t%f\t%f\n",j+1,positions[j].x,positions[j].y,positions[j].z);

	for(i = 1; i <= cycles; i++){
		compute_accelerations();
		compute_posistions();
		compute_velocities();
		compute_collisions();
		//print every 100 cycles and last cycle
		if(i % 100 == 0 || i == cycles){
			printf("\nCycle %d\n",i);
			for(j = 0; j < N; j++){
				printf("Body %d: \t%f\t%f\t%f\n",j+1,positions[j].x,positions[j].y,positions[j].z);
			}
		}
	}
}

