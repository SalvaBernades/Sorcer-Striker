#include "Enemy_Tank.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleParticles.h"

Enemy_Tank::Enemy_Tank(int x, int y) : Enemy(x, y)
{
	shoot = false;

	movingLeft.PushBack({ 40,40,37,35 });
	movingLeft.speed = 0.2f;
	movingLeft.loop = false;

	rollingLeft.PushBack({ 0,0,37,37 });
	rollingLeft.PushBack({ 0,39,37,36 });
	rollingLeft.speed = 0.03f;
	rollingLeft.loop = false;

	rollingRight.PushBack({ 80,0,37,37 });
	rollingRight.PushBack({ 80,39,37,36 });
	rollingRight.speed = 0.03f;
	rollingRight.loop = false;

	movingRight.PushBack({ 40, 2,37,35 });
	movingRight.speed = 0.2f;
	movingRight.loop = false;

	collider = App->collisions->AddCollider({ 0, 0, 35, 31 }, Collider::Type::ENEMY, (Module*)App->enemies);

	lives = 5;
	shooted = false;
	shootDelay.setTimer(2000);
}

void Enemy_Tank::Update()
{
	path.Update();
	currentAnim = path.GetCurrentAnimation();
//	spawnPos.y -= App->render->cameraSpeed;
	position = spawnPos + path.GetRelativePosition();

	shootDelay.refreshTimer();
	if (shootDelay.hasCompleted()) {

		if (!shooted) {
			shoot = App->particles->AddParticle(App->particles->dragronShoot,
				position.x + (currentAnim->GetCurrentFrame().w / 2),
				position.y + (currentAnim->GetCurrentFrame().h / 2),
				Collider::Type::ENEMY_SHOT);
			shoot->SpwanPos.x = position.x + (currentAnim->GetCurrentFrame().w / 2) - (shoot->anim.GetCurrentFrame().w / 2);
			shoot->SpwanPos.y = position.y + (currentAnim->GetCurrentFrame().h / 2) - (shoot->anim.GetCurrentFrame().h / 2);
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
				shoot->path.PushBack({ (direction.x), (direction.y) * -4.0f }, 25, &shoot->anim);
			}
			else {
				shoot->path.PushBack({ (direction.x), 4.0f }, 25, &shoot->anim);
			}
			shoot = nullptr;
			shootDelay.resetTimer();
			shooted = true;
			if (shootMargin < 0) {
				path.PushBack({ 0.0f,0.0f }, 10, &rollingLeft);
			}else{
				path.PushBack({ 0.0f,0.0f }, 10, &rollingRight);
			}
			shootDelay.setTimer(750);
		}
		else {
			shoot = App->particles->AddParticle(App->particles->dragronShoot,
				position.x + (currentAnim->GetCurrentFrame().w / 2),
				position.y + (currentAnim->GetCurrentFrame().h / 2),
				Collider::Type::ENEMY_SHOT);
			shoot->SpwanPos.x = position.x + (currentAnim->GetCurrentFrame().w / 2) - (shoot->anim.GetCurrentFrame().w / 2) + shootMargin;
			shoot->SpwanPos.y = position.y + (currentAnim->GetCurrentFrame().h / 2) - (shoot->anim.GetCurrentFrame().h / 2) - 5;
			shoot->pathing = true;
			// direction's calculation
			fPoint player = { (float)App->player->position.x + (App->player->playerWidth / 2),
				(float)App->player->position.y + (App->player->playerHeigth / 2) };
			fPoint bola = { (float)shoot->SpwanPos.x + (shoot->anim.GetCurrentFrame().w / 2),
				(float)shoot->SpwanPos.y + (shoot->anim.GetCurrentFrame().h / 2) };
			fPoint direction;
			direction = (player - bola);
			direction.Unit();
			if (player.y > bola.y) {
				shoot->path.PushBack({ (direction.x), 1.5f }, 25, &shoot->anim);
			}
			else {
				shoot->path.PushBack({ (direction.x), 4.0f }, 25, &shoot->anim);
			}
			shoot = nullptr;
			shootDelay.setTimer(5000);
		}
	}

	if (lives <= 0) {
		App->player->score += 50;
	}

	Enemy::Update();
}

void Enemy_Tank::createPathing(int pathnum)
{
	switch (pathnum)
	{
	case 0:
		path.PushBack({ -1.0f, 0.0f }, 100, &movingLeft);
		path.PushBack({ 0.0f, 0.0f }, 10, &movingLeft);
		path.loop = false;
		shootMargin = -15;
		break;
	case 1:
		path.PushBack({ 1.0f, 0.0f }, 100, &movingRight);
		path.PushBack({ 0.0f, 0.0f }, 10, &movingRight);
		path.loop = false;
		shootMargin = 15;
		break;
	default:
		break;
	}
	pathing = pathnum;
}
