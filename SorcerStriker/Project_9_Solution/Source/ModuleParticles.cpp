#include "ModuleParticles.h"

#include "Application.h"

#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleCollisions.h"

#include "SDL/include/SDL_timer.h"

ModuleParticles::ModuleParticles(bool startEnabled) : Module(startEnabled)
{
	for(uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		particles[i] = nullptr;

	// Explosion particle
	explosion.anim.PushBack({ 118, 50, 32, 34 });
	explosion.anim.PushBack({ 157, 6, 32, 33 });
	explosion.anim.PushBack({ 160, 52, 32, 32 });
	explosion.anim.loop = false;
	explosion.anim.speed = 0.15f;

	laser.anim.PushBack({ 0, 0, 4, 17 });
	laser.speed.y = -20;
	laser.lifetime = 35;

	laserExplosion.anim.PushBack({ 0, 26, 4, 5 });
	laserExplosion.anim.PushBack({ 7, 26, 4, 4 });
	laserExplosion.anim.PushBack({ 14, 27, 4, 3 });
	laserExplosion.anim.PushBack({ 22, 27, 2, 2 });
	laserExplosion.anim.loop = false;
	laserExplosion.anim.speed = 0.3f;

	sword.anim.PushBack({ 91, 2, 12, 84 });
	sword.speed.y = -20;
	sword.lifetime = 50;

	witchfireball.anim.PushBack({ 8, 0, 22, 24 });
	witchfireball.anim.PushBack({ 30, 0, 23, 25 });
	witchfireball.anim.PushBack({ 53, 0, 26, 25 });
	witchfireball.anim.loop = false;
	witchfireball.lifetime = 200;
	witchfireball.anim.speed = 0.03f;

	witchfireballreleased.anim.PushBack({ 30, 0, 23, 25 });
	witchfireballreleased.anim.PushBack({ 53, 0, 26, 25 });
	witchfireballreleased.anim.loop = true;
	witchfireballreleased.lifetime = 200;
	witchfireballreleased.anim.speed = 0.05f;

}

ModuleParticles::~ModuleParticles()
{

}

bool ModuleParticles::Start()
{
	LOG("Loading particles");
	texture = App->textures->Load("Assets/Sprites/proyectiles.png");

	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (particles[i] != nullptr)
		{
			delete particles[i];
			particles[i] = nullptr;
		}
	}

	return true;
}

Update_Status ModuleParticles::PreUpdate()
{
	// Remove all particles scheduled for deletion
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (particles[i] != nullptr && particles[i]->pendingToDelete)
		{
 			delete particles[i];
			particles[i] = nullptr;
		}
	}

	return Update_Status::UPDATE_CONTINUE;
}

bool ModuleParticles::CleanUp()
{
	LOG("Unloading particles");

	// Delete all remaining active particles on application exit 
	for(uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if(particles[i] != nullptr)
		{
			delete particles[i];
			particles[i] = nullptr;
		}
	}
	App->textures->Unload(texture);
	texture = nullptr;

	return true;
}

void ModuleParticles::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		// Always destroy particles that collide
		if (particles[i] != nullptr && particles[i]->collider == c1)
		{
			particles[i]->pendingToDelete = true;
			particles[i]->collider->pendingToDelete = true;
			if (particles[i]->collider->type = Collider::Type::PLAYER_SHOT) {
				App->particles->AddParticle(laserExplosion, particles[i]->position.x, particles[i]->position.y, Collider::Type::NONE);
			}
			break;
		}
	}
}

Update_Status ModuleParticles::Update()
{
	for(uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* particle = particles[i];

		if(particle == nullptr)	continue;

		// Call particle Update. If it has reached its lifetime, destroy it
		if(particle->Update() == false)
		{
			particles[i]->SetToDelete();
		}
	}

	return Update_Status::UPDATE_CONTINUE;
}

Update_Status ModuleParticles::PostUpdate()
{
	//Iterating all particle array and drawing any active particles
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* particle = particles[i];

		if (particle != nullptr && particle->isAlive)
		{
			App->render->Blit(texture, particle->position.x, particle->position.y, &(particle->anim.GetCurrentFrame()));
		}
	}

	return Update_Status::UPDATE_CONTINUE;
}

Particle* ModuleParticles::AddParticle(const Particle& particle, int x, int y, Collider::Type colliderType, uint delay)
{
	Particle* newParticle = nullptr;

	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		//Finding an empty slot for a new particle
		if (particles[i] == nullptr)
		{
			newParticle = new Particle(particle);
			newParticle->frameCount = -(int)delay;			// We start the frameCount as the negative delay
			newParticle->position.x = x;						// so when frameCount reaches 0 the particle will be activated
			newParticle->position.y = y;

			//Adding the particle's collider
			if (colliderType != Collider::Type::NONE)
				newParticle->collider = App->collisions->AddCollider(newParticle->anim.GetCurrentFrame(), colliderType, this);

			particles[i] = newParticle;
			break;
		}
	}

	return newParticle;
}