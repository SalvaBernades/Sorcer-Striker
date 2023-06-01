#pragma once
#include "Enemy.h"
#include "Path.h"
#include "Timer.h"
#include "Particle.h"
class Enemy_Tank :
    public Enemy
{
public:
	// Constructor (x y coordinates in the world)
	// Creates animation and movement data and the collider
	Enemy_Tank(int x, int y);

	// The enemy is going to follow the different steps in the path
	// Position will be updated depending on the speed defined at each step
	void Update() override;

	void createPathing(int pathnum) override;

private:
	// A set of steps that define the position in the screen
	// And an animation for each step
	Path path;
	Timer shootDelay;

	// This enemy has one sprite and one frame
	// We are keeping it an animation for consistency with other enemies
	Animation movingLeft;
	Animation movingRight;
	Animation rollingLeft;
	Animation rollingRight;

	int shootMargin;

	bool shooted;

	Particle* shoot;
};

