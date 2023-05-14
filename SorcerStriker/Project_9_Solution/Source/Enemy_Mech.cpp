#include "Enemy_Mech.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"

Enemy_Mech::Enemy_Mech(int x, int y) : Enemy(x, y)
{
	front.PushBack({0, 0, 100, 112});
	front.PushBack({ 100, 0, 100, 112 });
	front.speed = 0.03f;
	currentAnim = &front;
	//front.pingpong = true;
	lives = 10;
	path.PushBack({ 0.0f, 2.0f }, 125, &front);
	path.PushBack({ 0.0f, 0.0f }, 165, &front);
	path.PushBack({ 0.0f, 2.0f }, 85, &front);
	path.PushBack({ 0.0f, 0.0f }, 50, &front);
	path.PushBack({ 0.0f, -2.0f }, 550, &front);
	path.PushBack({ 0.0f, 2000.0f }, 10, &front);
	path.loop = false;

	collider = App->collisions->AddCollider({0, 0, 100, 112}, Collider::Type::ENEMY, (Module*)App->enemies);
}

void Enemy_Mech::Update()
{
	path.Update();
	spawnPos.y -= App->render->cameraSpeed;
	position = spawnPos + path.GetRelativePosition();
	currentAnim = path.GetCurrentAnimation();

	if (lives <= 0) {
		App->player->score += 500;
		if (collider != nullptr) {
			collider->pendingToDelete = true;
			collider = nullptr;
		}
	}

	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Enemy::Update();
}
