#include "Enemy_BrownShip.h"

#include "Application.h"
#include "ModuleCollisions.h"

Enemy_BrownShip::Enemy_BrownShip(int x, int y) : Enemy(x, y)
{
	idle.PushBack({ 0,0,42,43 });

	fly.PushBack({0,0,42,43});
	fly.PushBack({ 60,0,42,43 });
	fly.PushBack({ 119,0,42,43 });
	fly.PushBack({ 173,0,42,43 });
	fly.PushBack({ 223,0,42,43 });
	fly.speed = 0.1f;
	currentAnim = &idle;
	
	path.PushBack({-0.5f, -0.5f}, 200);
	path.PushBack({0.5f, -0.5f}, 80);
	path.PushBack({-1.0f, -1.0f}, 80);
	path.PushBack({ 1.0f, -0.5f }, 200);
	path.PushBack({ 0.5f, -0.5f }, 80);
	
	collider = App->collisions->AddCollider({0, 0, 42, 43}, Collider::Type::ENEMY, (Module*)App->enemies);
}

void Enemy_BrownShip::Update()
{
	path.Update();
	position = spawnPos + path.GetRelativePosition();

	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Enemy::Update();
}
