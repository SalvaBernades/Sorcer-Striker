#include "Boost.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"

Boost::Boost(int x, int y) : position(x, y)
{
	spawnPos = position;
}

Boost::~Boost()
{
	if (collider != nullptr)
		collider->pendingToDelete = true;
}

const Collider* Boost::GetCollider() const
{
	return collider;
}

void Boost::Update()
{
	if (currentAnim != nullptr)
		currentAnim->Update();

	if (collider != nullptr)
		collider->SetPos(position.x, position.y);
}

void Boost::Draw()
{
	if (currentAnim != nullptr) {
		if (taken) {
			App->render->Blit(textureBoost, position.x, position.y, &(currentAnim->GetCurrentFrame()));
		}
		else {
			App->render->Blit(textureBoostBox, position.x, position.y, &(currentAnim->GetCurrentFrame()));
		}
	}
}

void Boost::OnCollision(Collider* collider) 
{
	taken = true;
	App->audio->PlayFx(destroyedFx);

	SetToDelete();
}

void Boost::SetToDelete()
{
	pendingToDelete = true;
	if (collider != nullptr)
		collider->pendingToDelete = true;

	collider = nullptr;
}

void Boost::Shoot()
{

}

