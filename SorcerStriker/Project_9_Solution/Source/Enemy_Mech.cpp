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
	path.PushBack({ 0.0f, 0.0f }, 330, &front);
	path.PushBack({ 0.0f, 2.0f }, 85, &front);
	path.PushBack({ 0.0f, 0.0f }, 100, &front);
	path.PushBack({ 0.0f, -1.0f }, 550, &front);
	path.PushBack({ 0.0f, 2000.0f }, 10, &front);
	path.loop = false;
	State = Drake_States::FRONT;
	switchState.setTimer(3000);
	shootDelay.setTimer(150);
	maxShoots = 5;

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

	switch (State)
	{
	case Drake_States::FRONT:
		switchState.refreshTimer();
		if (switchState.hasCompleted()) {
			State = Drake_States::SHOOTING;
			switchState.resetTimer();
			shootDelay.refreshTimer();
		}
		break;
	case Drake_States::SHOOTING:
		shootDelay.refreshTimer();
		if (shootDelay.hasCompleted()) {
			// changing spot of shooting
			int x = maxShoots % 2;
			if (x == 0) {
				shoot = App->particles->AddParticle(App->particles->dragronShoot,
					position.x + (currentAnim->GetCurrentFrame().w / 2) + 15,
					position.y + (currentAnim->GetCurrentFrame().h / 2) + 15,
					Collider::Type::ENEMY_SHOT);
				shoot->SpwanPos.x = position.x + (currentAnim->GetCurrentFrame().w / 2) - (shoot->anim.GetCurrentFrame().w / 2) + 15;
				shoot->SpwanPos.y = position.y + (currentAnim->GetCurrentFrame().h / 2) - (shoot->anim.GetCurrentFrame().h / 2) + 15;
			}
			else {
				shoot = App->particles->AddParticle(App->particles->dragronShoot,
					position.x + (currentAnim->GetCurrentFrame().w / 2) - 15,
					position.y + (currentAnim->GetCurrentFrame().h / 2) + 15,
					Collider::Type::ENEMY_SHOT);
				shoot->SpwanPos.x = position.x + (currentAnim->GetCurrentFrame().w / 2) - (shoot->anim.GetCurrentFrame().w / 2) - 15;
				shoot->SpwanPos.y = position.y + (currentAnim->GetCurrentFrame().h / 2) - (shoot->anim.GetCurrentFrame().h / 2) + 15;
			}
			shoot->pathing = true;
			// direction's calculation
			fPoint player = { (float)App->player->position.x + (App->player->playerWidth / 2),
				(float)App->player->position.y + (App->player->playerHeigth / 2) - 50 };
			fPoint bola = { (float)shoot->SpwanPos.x + (shoot->anim.GetCurrentFrame().w / 2),
				(float)shoot->SpwanPos.y + (shoot->anim.GetCurrentFrame().h / 2) };
			fPoint direction;
			direction = (player - bola);
			direction.Unit();
			if (player.y < bola.y) {
				shoot->path.PushBack({ (direction.x), (direction.y ) * 4.0f}, 25, &shoot->anim);
			}
			else {
				shoot->path.PushBack({ (direction.x), (direction.y) * 0.05f }, 25, &shoot->anim);
			}
			shoot = nullptr;
			maxShoots--;
			shootDelay.resetTimer();
			if (maxShoots <= 0) {
				State = Drake_States::FRONT;
				maxShoots = 5;
			}
		}
		break;
	default:
		break;
	}

	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Enemy::Update();
}
