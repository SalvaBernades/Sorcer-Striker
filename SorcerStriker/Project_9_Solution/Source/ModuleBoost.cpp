#include "ModuleBoost.h"

#include "Application.h"

#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"

#include "Boost.h"
#include "Boost_LaserFist.h"

#define SPAWN_MARGIN 50


ModuleBoost::ModuleBoost(bool startEnabled) : Module(startEnabled)
{
	for (uint i = 0; i < MAX_BOOST; ++i)
		Boosts[i] = nullptr;
}

ModuleBoost::~ModuleBoost()
{

}

bool ModuleBoost::Start()
{
	textureBox = App->textures->Load("Assets/Sprites/Boost_Box_animacion.png");
	textureBoost = App->textures->Load("Assets/Sprites/Punos_Power_UP.png");
	boostFX = App->audio->LoadFx("Assets/Fx/Todos los power ups.wav");

	return true;
}


Update_Status ModuleBoost::PreUpdate()
{
	// Remove all enemies scheduled for deletion
	for (uint i = 0; i < MAX_BOOST; ++i)
	{
		if (Boosts[i] != nullptr && Boosts[i]->pendingToDelete && !Boosts[i]->taken)
		{
			delete Boosts[i];
			Boosts[i] = nullptr;
		}
	}

	return Update_Status::UPDATE_CONTINUE;
}

Update_Status ModuleBoost::Update()
{
	HandleBoostSpawn();

	for (uint i = 0; i < MAX_BOOST; ++i)
	{
		if (Boosts[i] != nullptr)
			Boosts[i]->Update();
	}

	HandleBoostDespawn();

	return Update_Status::UPDATE_CONTINUE;
}

Update_Status ModuleBoost::PostUpdate()
{
	for (uint i = 0; i < MAX_BOOST; ++i)
	{
		if (Boosts[i] != nullptr)
			Boosts[i]->Draw();
	}

	return Update_Status::UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleBoost::CleanUp()
{
	LOG("Freeing all enemies");

	for (uint i = 0; i < MAX_BOOST; ++i)
	{
		if (Boosts[i] != nullptr)
		{
			delete Boosts[i];
			Boosts[i] = nullptr;
		}
	}

	App->textures->Unload(textureBox);
	App->textures->Unload(textureBoost);
	textureBox = nullptr;
	textureBoost = nullptr;
	App->audio->Unload(boostFX);

	return true;
}

bool ModuleBoost::AddBoost(Boost_Type type, int x, int y)
{
	bool ret = false;

	for (uint i = 0; i < MAX_BOOST; ++i)
	{
		if (spawnQueue[i].type == Boost_Type::NO_TYPE)
		{
			spawnQueue[i].type = type;
			spawnQueue[i].x = x;
			spawnQueue[i].y = y;
			ret = true;
			break;
		}
	}

	return ret;
}

void ModuleBoost::HandleBoostSpawn()
{
	// Iterate all the enemies queue
	for (uint i = 0; i < MAX_BOOST; ++i)
	{
		if (spawnQueue[i].type != Boost_Type::NO_TYPE)
		{
			// Spawn a new enemy if the screen has reached a spawn position
			if (spawnQueue[i].y * SCREEN_SIZE > App->render->camera.y - SPAWN_MARGIN)
			{
				LOG("Spawning enemy at %d", spawnQueue[i].y * SCREEN_SIZE);

				SpawnBoost(spawnQueue[i]);
				spawnQueue[i].type = Boost_Type::NO_TYPE; // Removing the newly spawned enemy from the queue
			}
		}
	}
}

void ModuleBoost::HandleBoostDespawn()
{
	// Iterate existing enemies
	for (uint i = 0; i < MAX_BOOST; ++i)
	{
		if (Boosts[i] != nullptr)
		{
			// Delete the enemy when it has reached the end of the screen
			if (Boosts[i]->position.y * SCREEN_SIZE > App->render->camera.y + (App->render->camera.h * SCREEN_SIZE) + SPAWN_MARGIN)
			{
				LOG("DeSpawning enemy at %d", Boosts[i]->position.y * SCREEN_SIZE);

				Boosts[i]->SetToDelete();
			}
		}
	}
}

void ModuleBoost::Shoot()
{
	for (uint i = 0; i < MAX_BOOST; ++i)
	{
		if (Boosts[i] != nullptr)
		{
			Boosts[i]->Shoot(); //Notify the enemy of a collision
			break;
		}
	}
}

void ModuleBoost::SpawnBoost(const BoostSpawnpoint& info)
{
	// Find an empty slot in the enemies array
	for (uint i = 0; i < MAX_BOOST; ++i)
	{
		if (Boosts[i] == nullptr)
		{
			switch (info.type)
			{
			case Boost_Type::LASERFIST:
				Boosts[i] = new Boost_LaserFist(info.x, info.y);
				break;
			}
 			Boosts[i]->textureBoostBox = textureBox;
			Boosts[i]->textureBoost = textureBoost;
			Boosts[i]->destroyedFx = boostFX;
			break;
		}
	}
}

void ModuleBoost::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_BOOST; ++i)
	{
		if (Boosts[i] != nullptr && Boosts[i]->GetCollider() == c1)
		{
			Boosts[i]->OnCollision(c2); //Notify the enemy of a collision
			break;
		}
	}
}