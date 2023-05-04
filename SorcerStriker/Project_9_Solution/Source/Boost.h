#ifndef __BOOST_H__
#define __BOOST_H__

#include "p2Point.h"
#include "Animation.h"

struct SDL_Texture;
struct Collider;

class Boost
{
public:
	// Constructor
	// Saves the spawn position for later movement calculations
	Boost(int x, int y);

	// Destructor
	virtual ~Boost();

	// Returns the enemy's collider
	const Collider* GetCollider() const;

	// Called from inhering enemies' Udpate
	// Updates animation and collider position
	virtual void Update();

	// Called from ModuleEnemies' Update
	virtual void Draw();

	// Collision response
	virtual void OnCollision(Collider* collider);

	// Sets flag for deletion and for the collider aswell
	virtual void SetToDelete();

	virtual void Shoot();

public:
	// The current position in the world
	iPoint position;

	// If it has been taken
	bool taken = false;

	// The enemy's texture
	SDL_Texture* textureBoostBox = nullptr;
	SDL_Texture* textureBoost = nullptr;

	// Sound fx when destroyed
	int destroyedFx = 0;

	// A flag for the enemy removal. Important! We do not delete objects instantly
	bool pendingToDelete = false;

protected:
	// A ptr to the current animation
	Animation* currentAnim = nullptr;

	// The enemy's collider
	Collider* collider = nullptr;

	// Original spawn position. Stored for movement calculations
	iPoint spawnPos;
};

#endif // __BOOST_H__