#include "Enemy_BrownShip.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"

Enemy_BrownShip::Enemy_BrownShip(int x, int y) : Enemy(x, y)
{
	idle.PushBack({ 0,0,42,43 });

	fly.PushBack({0,0,42,43});
	fly.PushBack({ 60,0,42,43 });
	fly.PushBack({ 119,0,42,43 });
	fly.PushBack({ 173,0,42,43 });
	fly.PushBack({ 223,0,42,43 });
	fly.speed = 0.2f;
	fly.loop = false;
	created = false;
	shoot = false;

	currentAnim = &idle;
	
	collider = App->collisions->AddCollider({0, 0, 42, 43}, Collider::Type::ENEMY, (Module*)App->enemies);

	lives = 2;
}

void Enemy_BrownShip::Update()
{
	path.Update();
	currentAnim = path.GetCurrentAnimation();
	spawnPos.y -= App->render->cameraSpeed;
	position = spawnPos + path.GetRelativePosition();

	if (!created) {
		// casting particle
		fireball = App->particles->AddParticle(App->particles->witchfireball,
			position.x + (currentAnim->GetCurrentFrame().w / 2),
			position.y + (currentAnim->GetCurrentFrame().h / 2),
			Collider::Type::ENEMY_SHOT, 9);
		created = true;
	}
	if (!shoot && fireball->isAlive) {
		fireball->position.x = position.x + (currentAnim->GetCurrentFrame().w / 2) - (fireball->anim.GetCurrentFrame().w / 2);
		fireball->position.y = position.y + (currentAnim->GetCurrentFrame().h / 2) - (fireball->anim.GetCurrentFrame().h / 2) + 20;

		if (fireball->anim.HasFinished()){
			// realised particle
			fireballreleased = App->particles->AddParticle(App->particles->witchfireballreleased,
				position.x + (currentAnim->GetCurrentFrame().w / 2),
				position.y + (currentAnim->GetCurrentFrame().h / 2) + 20,
				Collider::Type::ENEMY_SHOT);
			fireballreleased->SpwanPos.x = position.x + (currentAnim->GetCurrentFrame().w / 2) - (fireball->anim.GetCurrentFrame().w / 2);
			fireballreleased->SpwanPos.y = position.y + (currentAnim->GetCurrentFrame().h / 2) - (fireball->anim.GetCurrentFrame().h / 2) + 20;
			fireballreleased->pathing = true;
			// direction's calculation
			fPoint player = { (float)App->player->position.x + (App->player->playerWidth/2),
				(float)App->player->position.y + (App->player->playerHeigth / 2)};
			fPoint bola = { (float)fireballreleased->SpwanPos.x + (fireballreleased->anim.GetCurrentFrame().w/2), 
				(float)fireballreleased->SpwanPos.y + (fireballreleased->anim.GetCurrentFrame().h / 2) };
			if (player.y > bola.y) {
				player.y -= 50.0f;
			}
			fPoint direction;
			direction = (player - bola);
			direction.Unit();
			if (player.y < bola.y) {
				fireballreleased->path.PushBack({ (direction.x * 2.0f), (direction.y * 4.0f) }, 25, &fireballreleased->anim);
			}
			else {
				fireballreleased->path.PushBack({ (direction.x * 2.0f), (direction.y * 2.0f) }, 25, &fireballreleased->anim);
			}
			shoot = true;
			// realease both painters and delete cast particle
			fireball->SetToDelete();
			fireball = nullptr;
			fireballreleased = nullptr;
		}
	}
	
	if (lives <= 0) {
			App->player->score += 50;
			if (collider != nullptr) {
				collider->pendingToDelete = true;
				collider = nullptr;
			}
			if (!shoot) {
				if (!fireball->pendingToDelete) {
					fireball->SetToDelete();
				}
				fireball = nullptr;
			}
	}

	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Enemy::Update();
}

void Enemy_BrownShip::createPathing(int pathnum)
{
	switch (pathnum)
	{
	case 0:
		path.PushBack({ 0.0f, 1.0f }, 100, &idle);
		path.PushBack({ 0.0f, 0.0f }, 25, &idle);
		path.PushBack({ 1.0f, -0.5f }, 30, &fly);
		path.PushBack({ 0.0f, -1.0f }, 75, &fly);
		path.PushBack({ 300.0f, 0.0f }, 1, &fly);
		path.loop = false;
		break;
	default:
		break;
	}
	pathing = pathnum;
}