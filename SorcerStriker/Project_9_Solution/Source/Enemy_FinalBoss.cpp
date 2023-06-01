#include "Enemy_FinalBoss.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleRender.h"

Enemy_FinalBoss::Enemy_FinalBoss(int x, int y) : Enemy(x, y)
{
	state = States::FULLHP;
	lives = 30;

	Boss = true;

	idle.PushBack({ 0,0,255,121 });
	idle.PushBack({ 257,0,255,121 });
	idle.PushBack({ 0,0,255,121 });
	idle.PushBack({ 257,0,255,121 });
	idle.PushBack({ 0,0,255,121 });
	idle.PushBack({ 257,0,255,121 });
	idle.PushBack({ 0,123,255,121 });
	idle.PushBack({ 257,123,255,121 });
	idle.PushBack({ 0,123,255,121 });
	idle.PushBack({ 257,123,255,121 });
	idle.PushBack({ 0,123,255,121 });
	idle.PushBack({ 257,123,255,121 });
	idle.speed = 0.2f;
	idle.loop = true;

	collider = App->collisions->AddCollider({ 0, 0, 33, 47 }, Collider::Type::ENEMY, (Module*)App->enemies);
	leftHead = App->collisions->AddCollider({ 0, 0, 31, 46 }, Collider::Type::ENEMY, (Module*)App->enemies);
}

void Enemy_FinalBoss::Update()
{
	path.Update();
	currentAnim = path.GetCurrentAnimation();
	spawnPos.y -= App->render->cameraSpeed;
	position = spawnPos + path.GetRelativePosition();

	switch (state)
	{
	case States::FULLHP:
		collider->SetPos(position.x + 111, position.y + 74);
		leftHead->SetPos(position.x + 47, position.y + 70);
		break;
	case States::LEFTHEAD:
		collider->SetPos(position.x + 111, position.y + 74);
		break;
	case States::RIGHTHEAD:
		collider->SetPos(position.x + 111, position.y + 74);
		break;
	case States::ONEHEAD:
		collider->SetPos(position.x + 111, position.y + 74);
		break;
	default:
		break;
	}

}

void Enemy_FinalBoss::createPathing(int pathnum)
{
	path.PushBack({ 0.0f, 2.0f }, 100, &idle);
	path.PushBack({ 0.0f, 0.0f }, 100, &idle);
	path.loop = false;
}

void Enemy_FinalBoss::OnCollision(Collider* collider)
{
	if (this->collider == collider) {
		lives--;
	}
	else if (this->leftHead == collider) {
		lives--;
	}
	else {
		Boss = false;
	}
	
}
