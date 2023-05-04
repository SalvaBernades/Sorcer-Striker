#pragma once
#include "Boost.h"
class Boost_LaserFist :
    public Boost
{
public:
	// Constructor (x y coordinates in the world)
	// Creates animation and movement data and the collider
	Boost_LaserFist(int x, int y);

	// The enemy is going to follow the different steps in the path
	// Position will be updated depending on the speed defined at each step
	void Update() override;

	void Shoot() override;

private:
	// This enemy has one sprite and one frame
	// We are keeping it an animation for consistency with other enemies
	Animation box;
	Animation fistShooting;
	Animation idle;

	bool shooting = false;
};

