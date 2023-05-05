#include "FountainParticles.h"

double random(int low, int high) {
	return (rand() % (high - low + 1)) + low;
}

void FountainParticles::init(int maxParticles)
{
	auto releaseArray = [](auto* arrayName) { if (arrayName != nullptr) delete[] arrayName; };

	releaseArray(positions);
	releaseArray(velocities);
	releaseArray(accelerations);
	releaseArray(orientations);
	nbrOfParticles = 0;
	this->maxParticles = maxParticles;
	positions = new float[maxParticles * 3];
	velocities = new float[maxParticles * 3];
	accelerations = new float[maxParticles * 3];
	orientations = new float[maxParticles * 4];

	return;
}

void FountainParticles::generate(int maxNewParticles)
{
	if (maxNewParticles + nbrOfParticles < maxParticles) { // generate the particles
		for (int currentParticle = 0; currentParticle < maxNewParticles; ++currentParticle) {
			
			//sets x position to a random value between -5 and 5
			positions[(nbrOfParticles + currentParticle) * 3] = random(-5, 5);

			//y position is always at 12.0
			positions[(nbrOfParticles + currentParticle) * 3 + 1] = 12.0f;

			//sets z position to a random value between -5 and 5
			positions[(nbrOfParticles + currentParticle) * 3 + 2] = random(-5, 5);

			double angle = angleDistribution(generator) * 2 * 3.14159 - 3.14159;
			int randYVelocity = random(-2, -0.1);

			velocities[(nbrOfParticles + currentParticle) * 3] =  0;
			velocities[(nbrOfParticles + currentParticle) * 3 + 1] = 0;
			velocities[(nbrOfParticles + currentParticle) * 3 + 2] =  0;

			accelerations[(nbrOfParticles + currentParticle) * 3] = 0.0;
			accelerations[(nbrOfParticles + currentParticle) * 3 + 1] = randYVelocity - 0.025;
			accelerations[(nbrOfParticles + currentParticle) * 3 + 2] = 0.0;

			//setup to be a quaternion that can be fed in
			orientations[(nbrOfParticles + currentParticle) * 4] = 0;
			orientations[(nbrOfParticles + currentParticle) * 4 + 1] = 0;
			orientations[(nbrOfParticles + currentParticle) * 4 + 2] = 0;
			orientations[(nbrOfParticles + currentParticle) * 4 + 3] = 0;
		}
		nbrOfParticles += maxNewParticles;
	}
}

void FountainParticles::update(float timeStep)
{
	// TODO Auto-generated method stub
	float newPosition[3];
	float newVelocity[3];
	float newAcceleration[3];
	float newOrientation[4];

	for (int current = 0; current < nbrOfParticles * 3; current += 3) {
		newPosition[0] = positions[current] + velocities[current] * timeStep + accelerations[current];
		newPosition[1] = positions[current + 1] + velocities[current + 1];
		newPosition[2] = positions[current + 2] + velocities[current + 2] * timeStep + accelerations[current + 2];

		newVelocity[0] = velocities[current] + accelerations[current] * timeStep;
		newVelocity[1] = velocities[current+1] + accelerations[current+1] * timeStep;
		newVelocity[2] = velocities[current + 2] + accelerations[current + 2] * timeStep;

		for (int i = 0; i < 3; i++) {
			positions[current + i] = newPosition[i];
			velocities[current + i] = newVelocity[i];
		}
	}
}

void FountainParticles::compact()
{
	int current, i;

	for (current = 0; current < nbrOfParticles; ++current) {
		while (positions[current * 3 + 1]  < 0.0f && current < nbrOfParticles) {
			for (i = 0; i < 3; ++i) {
				positions[current * 3 + i] = positions[(nbrOfParticles - 1) * 3 + i];
				velocities[current * 3 + i] = velocities[(nbrOfParticles - 1) * 3 + i];
				accelerations[current * 3 + i] = accelerations[(nbrOfParticles - 1) * 3 + i];
			}
			for (i = 0; i < 4; ++i) {
				orientations[current * 4 + i] = orientations[(nbrOfParticles - 1) * 4 + i];
			}
			--nbrOfParticles;
		}
	}
}

FountainParticles::FountainParticles()
{
	positions = nullptr;
	velocities = nullptr;
	accelerations = nullptr;
	orientations = nullptr;
	nbrOfParticles = 0;
	maxParticles = 0;
}
