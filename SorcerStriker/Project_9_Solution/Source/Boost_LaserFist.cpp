#include "Boost_LaserFist.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModulePlayer.h"
#include "ModuleParticles.h"

Boost_LaserFist::Boost_LaserFist(int x, int y) : Boost(x, y)
{
	box.PushBack({ 0,0,16,21 });
	box.PushBack({ 18,0,16,21 });
	box.speed = 0.01f;
	currentAnim = &box;

	fistShooting.PushBack({ 0,0,57,20 });
	fistShooting.PushBack({ 59,0,57,20 });
	fistShooting.PushBack({ 118,0,57,20 });
	fistShooting.PushBack({ 177,0,57,20 });
	fistShooting.loop = false;
	fistShooting.speed = 0.1f;

	idle.PushBack({ 0,0,57,20 });

	collider = App->collisions->AddCollider({ 0, 0, 16, 21 }, Collider::Type::BOOST, (Module*)App->boost);
}

void Boost_LaserFist::Update()
{
	if (taken) {
		if (!shooting) {
			currentAnim = &idle;
		}
		position.y = App->player->position.y;
		position.x = App->player->position.x - 9;
		if (currentAnim == &fistShooting && fistShooting.HasFinished()) {
			currentAnim = &idle;
			shooting = false;
			fistShooting.Reset();
		}
	}
	Boost::Update();
}

void Boost_LaserFist::Shoot()
{
	if (taken) {
		if (currentAnim == &fistShooting) {
			fistShooting.Reset();
		}
		else {
			currentAnim = &fistShooting;
		}
		shooting = true;
		Particle* newParticle = App->particles->AddParticle(App->particles->sword, position.x, position.y - 60, Collider::Type::PLAYER_SHOT);
		newParticle->collider->AddListener(App->player);
		newParticle = App->particles->AddParticle(App->particles->sword, (position.x + 57) - 10, position.y - 60, Collider::Type::PLAYER_SHOT);
		newParticle->collider->AddListener(App->player);
		newParticle = nullptr;
	}
}
