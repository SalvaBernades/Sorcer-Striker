#include "Boost_Coin.h"
#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"

Boost_Coin::Boost_Coin(int x, int y) : Boost(x, y)
{
	flipping.PushBack({ 151,42,10,9 });
	flipping.PushBack({ 172,43,10,8 });
	flipping.PushBack({ 194,44,10,6 });
	flipping.PushBack({ 216,46,10,3 });
	flipping.PushBack({ 238,44,12,6 });
	flipping.PushBack({ 262,41,13,10 });
	flipping.speed = 0.1f;
	flipping.loop = true;
	this->currentAnim = &flipping;
	taken = false;

	GoOut.PushBack({ 0.0f,-2.0f }, 25);
	GoOut.PushBack({ 0.0f,2.0f }, 100);
	GoOut.loop = false;

	collider = App->collisions->AddCollider({ 0, 0, 12, 12 }, Collider::Type::BOOST, (Module*)App->boost);
}

void Boost_Coin::Update()
{
	GoOut.Update();
	position = spawnPos + GoOut.GetRelativePosition();
	Boost::Update();
	spawnPos.y -= App->render->cameraSpeed;
}

void Boost_Coin::OnCollision(Collider* collider)
{
	if (collider->type == Collider::Type::PLAYER) {
		App->player->score += 100;
		App->audio->PlayFx(destroyedFx);
		SetToDelete();
	}
}
