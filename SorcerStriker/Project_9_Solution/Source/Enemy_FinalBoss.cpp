#include "Enemy_FinalBoss.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"

Enemy_FinalBoss::Enemy_FinalBoss(int x, int y) : Enemy(x, y)
{
	state = States::FULLHP;
	lives = 30;
	lasthit = 0;
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
	currentAnim = &idle;

	LH.PushBack({ 0,249,255,121 });
	LH.PushBack({ 257,249,255,121 });
	LH.PushBack({ 0,249,255,121 });
	LH.PushBack({ 257,249,255,121 });
	LH.PushBack({ 0,249,255,121 });
	LH.PushBack({ 257,249,255,121 });
	LH.PushBack({ 0,372,255,121 });
	LH.PushBack({ 257,372,255,121 });
	LH.PushBack({ 0,372,255,121 });
	LH.PushBack({ 257,372,255,121 });
	LH.PushBack({ 0,372,255,121 });
	LH.PushBack({ 257,372,255,121 });
	LH.speed = 0.2f;
	LH.loop = true;

	RH.PushBack({ 0,497,255,121 });
	RH.PushBack({ 257,497,255,121 });
	RH.PushBack({ 0,497,255,121 });
	RH.PushBack({ 257,497,255,121 });
	RH.PushBack({ 0,497,255,121 });
	RH.PushBack({ 257,497,255,121 });
	RH.PushBack({ 0,619,255,121 });
	RH.PushBack({ 257,619,255,121 });
	RH.PushBack({ 0,619,255,121 });
	RH.PushBack({ 257,619,255,121 });
	RH.PushBack({ 0,619,255,121 });
	RH.PushBack({ 257,619,255,121 });
	RH.speed = 0.2f;
	RH.loop = true;

	OH.PushBack({ 0,746,255,121 });
	OH.PushBack({ 257,746,255,121 });
	OH.PushBack({ 0,746,255,121 });
	OH.PushBack({ 257,746,255,121 });
	OH.PushBack({ 0,746,255,121 });
	OH.PushBack({ 257,746,255,121 });
	OH.PushBack({ 0,869,255,121 });
	OH.PushBack({ 257,869,255,121 });
	OH.PushBack({ 0,869,255,121 });
	OH.PushBack({ 257,869,255,121 });
	OH.PushBack({ 0,869,255,121 });
	OH.PushBack({ 257,869,255,121 });
	OH.speed = 0.2f;
	OH.loop = true;

	shoots = 4;
	delaybts.setTimer(300);
	shootDelay.setTimer(3000);
	collider = App->collisions->AddCollider({ 0, 0, 33, 47 }, Collider::Type::ENEMY, (Module*)App->enemies);
	leftHead = App->collisions->AddCollider({ 0, 0, 31, 46 }, Collider::Type::ENEMY, (Module*)App->enemies);
	rigthHead = App->collisions->AddCollider({ 0, 0, 31, 46 }, Collider::Type::ENEMY, (Module*)App->enemies);
}

void Enemy_FinalBoss::Update()
{
	path.Update();
	currentAnim->Update();
	spawnPos.y -= App->render->cameraSpeed;
	position = spawnPos + path.GetRelativePosition();
	shootDelay.refreshTimer();
	delaybts.refreshTimer();

	switch (state)
	{
	case States::FULLHP:
		collider->SetPos(position.x + 111, position.y + 74);
		leftHead->SetPos(position.x + 47, position.y + 70);
		rigthHead->SetPos(position.x + 177, position.y + 70);
		if (lives < 20) {
			if (lasthit == 0) {
				state = States::LEFTHEAD;
				rigthHead->pendingToDelete = true;
				rigthHead = nullptr;
				currentAnim = &LH;
			}
			else if (lasthit == 1) {
				state = States::RIGHTHEAD;
				leftHead->pendingToDelete = true;
				leftHead = nullptr;
				currentAnim = &RH;
			}
			else {
				state = States::LEFTHEAD;
				rigthHead->pendingToDelete = true;
				rigthHead = nullptr;
				currentAnim = &LH;
			}
		}
		break;
	case States::LEFTHEAD:
		collider->SetPos(position.x + 111, position.y + 74);
		leftHead->SetPos(position.x + 47, position.y + 70);
		if (lives < 10) {
			state = States::ONEHEAD;
			leftHead->pendingToDelete = true;
			leftHead = nullptr;
			currentAnim = &OH;
		}
		break;
	case States::RIGHTHEAD:
		collider->SetPos(position.x + 111, position.y + 74);
		rigthHead->SetPos(position.x + 177, position.y + 70);
		if (lives < 10) {
			state = States::ONEHEAD;
			rigthHead->pendingToDelete = true;
			rigthHead = nullptr;
			currentAnim = &OH;
		}
		break;
	case States::ONEHEAD:
		collider->SetPos(position.x + 111, position.y + 74);
		if (lives <= 0) {
			collider->pendingToDelete = true;
			collider = nullptr;
			this->pendingToDelete = true;
			state = States::DEAD;
			App->player->score += 1000;
		}
		break;
	case States::DEAD:

		break;
	default:
		break;
	}

	if (shootDelay.hasCompleted()) {
		if (delaybts.hasCompleted()) {
			if (collider != nullptr) {
				fireball = App->particles->AddParticle(App->particles->finalbossfireball,
					position.x + 120,
					position.y + 90,
					Collider::Type::ENEMY_SHOT);
				fireball = nullptr;
			}
			if (rigthHead != nullptr) {
				fireball = App->particles->AddParticle(App->particles->finalbossfireball,
					position.x + 185,
					position.y + 85,
					Collider::Type::ENEMY_SHOT);
				fireball = nullptr;
			}
			if (leftHead != nullptr) {
				fireball = App->particles->AddParticle(App->particles->finalbossfireball,
					position.x + 55,
					position.y + 85,
					Collider::Type::ENEMY_SHOT);
				fireball = nullptr;
			}
			delaybts.resetTimer();
			shoots--;
		}
		if (shoots <= 0) {
			shootDelay.resetTimer();
			shoots = 4;
		}
	}

}

void Enemy_FinalBoss::createPathing(int pathnum)
{
	path.PushBack({ 0.0f, 3.0f }, 90);
	path.PushBack({ 0.0f, 0.0f }, 100);
	path.loop = false;
}

void Enemy_FinalBoss::OnCollision(Collider* collider)
{
	if (this->collider == collider) {
		lives--;
		lasthit = 0;
	}
	else if (this->leftHead == collider) {
		lives--;
		lasthit = 1;
	}
	else if (this->rigthHead == collider) {
		lives--;
		lasthit = 2;
	}
	else {
		Boss = false;
	}
	
}
