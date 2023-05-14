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
		fireball = App->particles->AddParticle(App->particles->witchfireball,
			position.x + (currentAnim->GetCurrentFrame().w / 2),
			position.y + (currentAnim->GetCurrentFrame().h / 2),
			Collider::Type::ENEMY_SHOT, 9);
		created = true;
	}
	if (!shoot) {
		fireball->position.x = position.x + (currentAnim->GetCurrentFrame().w / 2) - (fireball->anim.GetCurrentFrame().w / 2);
		fireball->position.y = position.y + (currentAnim->GetCurrentFrame().h / 2) - (fireball->anim.GetCurrentFrame().h / 2) + 20;
		if (fireball->anim.HasFinished()){
			fireballreleased = App->particles->AddParticle(App->particles->witchfireballreleased,
				position.x + (currentAnim->GetCurrentFrame().w / 2),
				position.y + (currentAnim->GetCurrentFrame().h / 2) + 20,
				Collider::Type::ENEMY_SHOT);
			fireballreleased->SpwanPos.x = position.x + (currentAnim->GetCurrentFrame().w / 2) - (fireball->anim.GetCurrentFrame().w / 2);
			fireballreleased->SpwanPos.y = position.y + (currentAnim->GetCurrentFrame().h / 2) - (fireball->anim.GetCurrentFrame().h / 2) + 20;
			fPoint player = { (float)App->player->position.x, (float)App->player->position.y };
			fPoint bola = { (float)fireballreleased->SpwanPos.x, (float)fireballreleased->SpwanPos.y };
			float distanceX = (player.operator-(bola).x);
			float distanceY = (player.operator-(bola).y);
			fPoint direction = { (bola.operator-(player).x) / bola.DistanceTo(player) ,(bola.operator-(player).y) / bola.DistanceTo(player) };
			shoot = true;
			fireballreleased->path.PushBack({ -(direction.x * 3.0f), -(direction.y * 3.0f) }, 25, &fireballreleased->anim);
			fireballreleased->pathing = true;
			fireball->SetToDelete();
			fireball->isAlive = false;
			fireball = nullptr;
		}
	}
	
	if (lives <= 0) {
			App->player->score += 50;
			if (collider != nullptr) {
				collider->pendingToDelete = true;
				collider = nullptr;
			}
		if (fireball != nullptr) {
			fireball->SetToDelete();
			fireball = nullptr;
		}
		if (fireballreleased != nullptr) {
			fireballreleased = nullptr;
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