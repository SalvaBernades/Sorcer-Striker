#include "Boost_CoinBag.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleRender.h"
#include "Boost_Coin.h"
#include "ModuleBoost.h"

Boost_CoinBag::Boost_CoinBag(int x, int y) : Boost(x, y)
{
	flying.PushBack({ 0,0,22,28 });
	flying.PushBack({ 32,1,33,28 });
	flying.PushBack({ 118,1,28,25 });
	flying.PushBack({ 75,2,31,24 });
	flying.speed = 0.1f;
	flying.loop = true;
	this->currentAnim = &flying;

	goingOut.PushBack({ 0,43,27,37 });
	goingOut.PushBack({ 32,51,38,28 });
	goingOut.PushBack({ 79,51,28,32 });
	goingOut.speed = 0.2f;
	goingOut.loop = true;

	GoIn.PushBack({ 0.0f, 2.0f }, 100);
	GoIn.PushBack({ 0.0f, 0.0f }, 100);
	GoIn.loop = false;

	GoOut.PushBack({ 0.0f, -2.0f }, 100);
	GoOut.PushBack({ 0.0f, 0.0f }, 100);
	GoOut.loop = false;

	taken = false;
	coins = 5;
	timeToDelete.setTimer(3000);
	shootDealy.setTimer(1500);
	empty = false;
	collider = App->collisions->AddCollider({ 0, 0, 18, 20 }, Collider::Type::BOOST, (Module*)App->boost);
}

void Boost_CoinBag::Update()
{
	if (taken) {
		GoOut.Update();
		position = spawnPos + GoOut.GetRelativePosition();
		timeToDelete.refreshTimer();
		if (!empty) {
			shootDealy.refreshTimer();
			if (shootDealy.hasCompleted()) {
				shootDealy.resetTimer();
				Shoot();
				coins--;
			}
			if (coins < 1)
			{
				empty = true;
			}
		}
		if (timeToDelete.hasCompleted()) {
			pendingToDelete = true;
			taken = false;
		}
	}
	else {
		GoIn.Update();
		position = spawnPos + GoIn.GetRelativePosition();
		if (coins > 0) {
			shootDealy.refreshTimer();
			if (shootDealy.hasCompleted()) {
				shootDealy.resetTimer();
				Shoot();
				coins--;
			}
		}
		if (coins < 1 && !empty) {
			empty = true;
			taken = true;
			timeToDelete.resetTimer();
			this->currentAnim = &goingOut;
			spawnPos = spawnPos + GoIn.GetRelativePosition();
			if (this->collider != nullptr) {
				this->collider->pendingToDelete = true;
				this->collider = nullptr;
			}
		}
	}
	spawnPos.y -= App->render->cameraSpeed;
	
	if (currentAnim != nullptr)
		currentAnim->Update();

	if (collider != nullptr)
		collider->SetPos(position.x + 6, position.y + 6);

}

void Boost_CoinBag::Shoot()
{
	App->boost->AddBoost(Boost_Type::COIN, position.x + 6, position.y + 6);//-1800
}

void Boost_CoinBag::OnCollision(Collider* collider)
{
	if (collider->type == Collider::Type::PLAYER_SHOT && taken == false) {
		taken = true;
		timeToDelete.resetTimer();
		this->currentAnim = &goingOut;
		spawnPos = spawnPos + GoIn.GetRelativePosition();
		this->collider->pendingToDelete = true;
		this->collider = nullptr;
		shootDealy.setTimer(150);
		shootDealy.resetTimer();
	}
}
