#include "Enemy.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"

Enemy::Enemy(int x, int y) : position(x, y)
{
	spawnPos = position;
	Boss = false;
}

Enemy::~Enemy()
{
	if (collider != nullptr)
		collider->pendingToDelete = true;
}

const Collider* Enemy::GetCollider() const
{
	return collider;
}

void Enemy::Update()
{
	if (currentAnim != nullptr)
		currentAnim->Update();

	if (collider != nullptr)
		collider->SetPos(position.x, position.y);
}

void Enemy::Draw()
{
	if (currentAnim != nullptr)
		App->render->Blit(texture, position.x, position.y, &(currentAnim->GetCurrentFrame()));
}

void Enemy::OnCollision(Collider* collider)
{
	lives--;
	if (lives <= 0) {
		App->particles->AddParticle(App->particles->explosion, position.x, position.y);
		App->audio->PlayFx(destroyedFx);

		SetToDelete();
	}
}

void Enemy::SetToDelete()
{
	pendingToDelete = true;
	if (collider != nullptr) {
		collider->pendingToDelete = true;
		collider = nullptr;
	}
}

void Enemy::createPathing(int pathing)
{
}
