#ifndef __ENEMY_MECH_H__
#define __ENEMY_MECH_H__

#include "Enemy.h"
#include "Path.h"
#include "Timer.h"
#include "ModuleParticles.h"

enum class Drake_States
{
	FRONT,
	SHOOTING,
};

class Enemy_Mech : public Enemy
{
public:
	// Constructor (x y coordinates in the world)
	// Creates animation and movement data and the collider
	Enemy_Mech(int x, int y);

	// The enemy is going to follow the different steps in the path
	// Position will be updated depending on the speed defined at each step
	void Update() override;

private:
	// The path that will define the position in the world
	Path path;

	// Enemy animations
	Animation front;

	Timer switchState;
	Timer shootDelay;
	int maxShoots = 5;
	Particle* shoot;

	Drake_States State;
};

#endif // __ENEMY_MECH_H__