#include "ModuleEnemies.h"

#include "Application.h"

#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"

#include "Enemy.h"
#include "Enemy_RedBird.h"
#include "Enemy_BrownShip.h"
#include "Enemy_Mech.h"
#include "Enemy_Tank.h"
#include "Enemy_FinalBoss.h"

#define SPAWN_MARGIN 50


ModuleEnemies::ModuleEnemies(bool startEnabled) : Module(startEnabled)
{
	for(uint i = 0; i < MAX_ENEMIES; ++i)
		enemies[i] = nullptr;
}

ModuleEnemies::~ModuleEnemies()
{

}

bool ModuleEnemies::Start()
{
	texture = App->textures->Load("Assets/Sprites/Emenies.png");
	enemyDestroyedFx = App->audio->LoadFx("Assets/Fx/explosion.wav");
	dragonstexture = App->textures->Load("Assets/Sprites/Dragones.png");
	tanktexture = App->textures->Load("Assets/Sprites/Tanques.png");
	finalboss = App->textures->Load("Assets/Sprites/Dragon_Final.png");

	return true;
}


Update_Status ModuleEnemies::PreUpdate()
{
	// Remove all enemies scheduled for deletion
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr && enemies[i]->pendingToDelete)
		{
			delete enemies[i];
			enemies[i] = nullptr;
		}
	}

	return Update_Status::UPDATE_CONTINUE;
}

Update_Status ModuleEnemies::Update()
{
	HandleEnemiesSpawn();

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(enemies[i] != nullptr)
			enemies[i]->Update();
	}

	HandleEnemiesDespawn();

	return Update_Status::UPDATE_CONTINUE;
}

Update_Status ModuleEnemies::PostUpdate()
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
			enemies[i]->Draw();
	}

	return Update_Status::UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleEnemies::CleanUp()
{
	LOG("Freeing all enemies");

	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(enemies[i] != nullptr)
		{
			delete enemies[i];
			enemies[i] = nullptr;
		}
		if (spawnQueue[i].type != Enemy_Type::NO_TYPE)
		{
			spawnQueue[i].type = Enemy_Type::NO_TYPE;
		}
	}

	App->textures->Unload(texture);
	App->textures->Unload(dragonstexture);
	App->textures->Unload(tanktexture);
	App->textures->Unload(finalboss);
	App->audio->Unload(enemyDestroyedFx);
	texture = nullptr;
	dragonstexture = nullptr;
	tanktexture = nullptr;
	finalboss = nullptr;

	return true;
}

bool ModuleEnemies::AddEnemy(Enemy_Type type, int x, int y, int pathing)
{
	bool ret = false;

	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(spawnQueue[i].type == Enemy_Type::NO_TYPE)
		{
			spawnQueue[i].type = type;
			spawnQueue[i].x = x;
			spawnQueue[i].y = y;
			spawnQueue[i].pathing = pathing;
			ret = true;
			break;
		}
	}

	return ret;
}

void ModuleEnemies::HandleEnemiesSpawn()
{
	// Iterate all the enemies queue
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (spawnQueue[i].type != Enemy_Type::NO_TYPE)
		{
			if (spawnQueue[i].type == Enemy_Type::MECH || spawnQueue[i].type == Enemy_Type::FINALBOSS) {
				if (spawnQueue[i].y * SCREEN_SIZE > App->render->camera.y - 120) {

					LOG("Spawning enemy at %d", spawnQueue[i].y * SCREEN_SIZE);

					SpawnEnemy(spawnQueue[i]);
					spawnQueue[i].type = Enemy_Type::NO_TYPE; // Removing the newly spawned enemy from the queue
				}
			}
			// Spawn a new enemy if the screen has reached a spawn position
			else {
				if (spawnQueue[i].y * SCREEN_SIZE > App->render->camera.y - SPAWN_MARGIN) {

					LOG("Spawning enemy at %d", spawnQueue[i].y * SCREEN_SIZE);

					SpawnEnemy(spawnQueue[i]);
					spawnQueue[i].type = Enemy_Type::NO_TYPE; // Removing the newly spawned enemy from the queue
				}
			}
		}
	}
}

void ModuleEnemies::HandleEnemiesDespawn()
{
	// Iterate existing enemies
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
		{
			// Delete the enemy when it has reached the end of the screen
			if (enemies[i]->position.y * SCREEN_SIZE > App->render->camera.y + (App->render->camera.h * SCREEN_SIZE) + SPAWN_MARGIN)
			{
				LOG("DeSpawning enemy at %d", enemies[i]->position.y * SCREEN_SIZE);

 				enemies[i]->SetToDelete();
			}
		}
	}
}

void ModuleEnemies::SpawnEnemy(const EnemySpawnpoint& info)
{
	// Find an empty slot in the enemies array
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] == nullptr)
		{
			switch (info.type)
			{
				case Enemy_Type::REDBIRD:
					enemies[i] = new Enemy_RedBird(info.x, info.y);
					enemies[i]->texture = texture;
					break;
				case Enemy_Type::BROWNSHIP:
					enemies[i] = new Enemy_BrownShip(info.x, info.y);
					enemies[i]->texture = texture;
					break;
				case Enemy_Type::MECH:
					enemies[i] = new Enemy_Mech(info.x, info.y);
					enemies[i]->texture = dragonstexture;
					break;
				case Enemy_Type::TANK:
					enemies[i] = new Enemy_Tank(info.x, info.y);
					enemies[i]->texture = tanktexture;
					break;
				case Enemy_Type::FINALBOSS:
					enemies[i] = new Enemy_FinalBoss(info.x, info.y);
					enemies[i]->texture = finalboss;
					break;
			}
			enemies[i]->createPathing(info.pathing);
			enemies[i]->destroyedFx = enemyDestroyedFx;
			break;
		}
	}
}

void ModuleEnemies::OnCollision(Collider* c1, Collider* c2)
{
	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(enemies[i] != nullptr && enemies[i]->GetCollider() == c1 && !enemies[i]->Boss)
		{
			enemies[i]->OnCollision(c2); //Notify the enemy of a collision
			break;
		}
		else if (enemies[i] != nullptr && enemies[i]->Boss) {
			if (c2->type != Collider::Type::PLAYER) {
				enemies[i]->OnCollision(c1);
				if (enemies[i]->Boss) {
					break;
				}
				else {
					enemies[i]->Boss = true;
				}
			}
		}
	}
}