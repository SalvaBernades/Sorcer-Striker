#pragma once
#include "Boost.h"
#include "Path.h"

class Boost_Coin :
    public Boost
{
public:
	// Constructor (x y coordinates in the world)
	// Creates animation and movement data and the collider
	Boost_Coin(int x, int y);

	// The enemy is going to follow the different steps in the path
	// Position will be updated depending on the speed defined at each step
	void Update() override;

	void OnCollision(Collider* collider) override;

private:
	// This enemy has one sprite and one frame
	// We are keeping it an animation for consistency with other enemies
	Animation flipping;
	Path GoOut;

};

