#include "Enemy_RedBird.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"

Enemy_RedBird::Enemy_RedBird(int x, int y) : Enemy(x, y)
{
	flyAnim.PushBack({301,43,24,35});
	flyAnim.PushBack({ 336,43,24,35 });

	flyAnim.speed = 0.3f;

	currentAnim = &flyAnim;

	collider = App->collisions->AddCollider({0, 0, 24, 35}, Collider::Type::ENEMY, (Module*)App->enemies);
	lives = 1;
}

void Enemy_RedBird::Update()
{

	path.Update();
	currentAnim = path.GetCurrentAnimation();
	spawnPos.y -= (App->render->cameraSpeed / 2);
	position = spawnPos + path.GetRelativePosition();
	if (pathing == 2) {
		waveRatio += waveRatioSpeed;
		position.x = spawnPos.x + (waveHeight * sinf(waveRatio));
	}

	if (lives <= 0) {
		App->player->score += 10;
	}

	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Enemy::Update();
}

void Enemy_RedBird::createPathing(int pathnum)
{
	switch (pathnum)
	{
	case 0:
		path.PushBack({ 2.0f, 3.0f }, 90, &flyAnim);
		path.PushBack({ -2.0f, 0.0f }, 15, &flyAnim);
		path.PushBack({ -2.0f, -5.0f }, 200, &flyAnim);
		path.PushBack({ 0.0, 2000.0f }, 1, &flyAnim);
		path.loop = false;
		break;
	case 1:
		path.PushBack({ -2.0f, 3.0f }, 90, &flyAnim);
		path.PushBack({ 2.0f, 0.0f }, 15, &flyAnim);
		path.PushBack({ 2.0f, -5.0f }, 200, &flyAnim);
		path.PushBack({ 0.0, 2000.0f }, 1, &flyAnim);
		path.loop = false;
		break;
	case 2:
		path.PushBack({ 0.0f, 1.0f }, 50, &flyAnim);
		break;
	case 3:
		path.PushBack({ 0.0f, 2.0f }, 55, &flyAnim);
		path.PushBack({ -2.0f, 2.0f }, 50, &flyAnim);
		path.PushBack({ 0.0f, -4.0f }, 100, &flyAnim);
		path.PushBack({ -2.0f, 2.0f }, 50, &flyAnim);
		path.PushBack({ -2.0f, -2.0f }, 50, &flyAnim);
		path.PushBack({ 0.0f, 2000.0f }, 1, &flyAnim);
		path.loop = false;
		break;
	default:
		break;
	}
	pathing = pathnum;
}
