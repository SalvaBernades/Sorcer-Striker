#ifndef __MODULE_BOOST_H__
#define __MODULE_BOOST_H__

#include "Module.h"

enum class Boost_Type
{
	NO_TYPE,
	LASERFIST,
	COINBAG,
	COIN,
};

#define MAX_BOOST 10

struct BoostSpawnpoint
{
	Boost_Type type = Boost_Type::NO_TYPE;
	int x, y;
};

class Boost;
struct SDL_Texture;

class ModuleBoost :
    public Module
{
public:
	// Constructor
	ModuleBoost(bool startEnabled);

	// Destructor
	~ModuleBoost();

	// Called when the module is activated
	// Loads the necessary textures for the enemies
	bool Start() override;

	// Called at the beginning of the application loop
	// Removes all enemies pending to delete
	Update_Status PreUpdate() override;

	// Called at the middle of the application loop
	// Handles all enemies logic and spawning/despawning
	Update_Status Update() override;

	// Called at the end of the application loop
	// Iterates all the enemies and draws them
	Update_Status PostUpdate() override;

	// Called on application exit
	// Destroys all active enemies left in the array
	bool CleanUp() override;

	// Called when an enemi collider hits another collider
	// The enemy is destroyed and an explosion particle is fired
	void OnCollision(Collider* c1, Collider* c2) override;

	// Add an enemy into the queue to be spawned later
	bool AddBoost(Boost_Type type, int x, int y);

	// Iterates the queue and checks for camera position
	void HandleBoostSpawn();

	// Destroys any enemies that have moved outside the camera limits
	void HandleBoostDespawn();

private:
	// Spawns a new enemy using the data from the queue
	void SpawnBoost(const BoostSpawnpoint& info);

private:
	// A queue with all spawn points information
	BoostSpawnpoint spawnQueue[MAX_BOOST];

	// All spawned enemies in the scene
	Boost* Boosts[MAX_BOOST] = { nullptr };

	// The enemies sprite sheet
	SDL_Texture* textureBox_LaserFist = nullptr;
	SDL_Texture* textureBoost_LaserFist = nullptr;
	SDL_Texture* texture_CoinBag = nullptr;

	// The audio fx for destroying an enemy
	int boostFX = 0;
};

#endif // __MODULE_BOOST_H__

