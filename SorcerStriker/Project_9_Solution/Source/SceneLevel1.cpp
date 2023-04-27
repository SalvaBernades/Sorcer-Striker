#include "SceneLevel1.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleCollisions.h"
#include "ModuleEnemies.h"
#include "ModulePlayer.h"
#include "ModuleParticles.h"

SceneLevel1::SceneLevel1(bool startEnabled) : Module(startEnabled)
{

}

SceneLevel1::~SceneLevel1()
{

}

// Load assets
bool SceneLevel1::Start()
{
	LOG("Loading background assets");

	bool ret = true;

	bgTexture = App->textures->Load("Assets/Sprites/Background-forest.png");
//	App->audio->PlayMusic("Assets/Music/stage1.ogg", 1.0f);
	uint w, h;
	bgRect[0].x = 0;
	bgRect[0].y = 0;
	App->textures->GetTextureSize(bgTexture, w, h);
	bgRect[1].x = 0;
	bgRect[1].y = bgRect[0].y - h;
	bgRect[0].w = w;
	bgRect[0].h = h;
	bgRect[1].w = w;
	bgRect[1].h = h;

	//Bottomside collider
	App->collisions->AddCollider({ 0, 224, 3930, 16 }, Collider::Type::WALL);

	//First two columns colliders
	App->collisions->AddCollider({ 1375, 0, 111, 96 }, Collider::Type::WALL);
	App->collisions->AddCollider({ 1375, 145, 111, 96 }, Collider::Type::WALL);

//Enemies ---
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 0, -200);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 100, -200);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -200);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 300, -200);
	
	
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 0, -300);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD,  100, -300);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD,  200, -300);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD,  300, -300);
	
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 0, -400);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 100, -400);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -400);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 300, -400);
	
	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 0, -100);
	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 0, -100);
	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 0, -100);
	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 0, -100);
	
	App->enemies->AddEnemy(Enemy_Type::MECH, 900, 195);

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	App->enemies->Enable();
	App->collisions->Enable();
	App->player->Enable();


	return ret;
}

Update_Status SceneLevel1::Update()
{
	App->render->camera.y -= 3;

	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background
Update_Status SceneLevel1::PostUpdate()
{
	// Draw everything --------------------------------------
	//App->render->Blit(bgTexture, 0, 0, NULL);


	App->render->Blit(bgTexture, 0, bgRect[0].y, NULL);
	App->render->Blit(bgTexture, 0, bgRect[1].y, NULL);

	if (bgRect[0].y * SCREEN_SIZE > App->render->camera.y + (App->render->camera.h * SCREEN_SIZE)) {
		bgRect[0].y = bgRect[1].y - bgRect[1].h;
	}
	else if (bgRect[1].y * SCREEN_SIZE > App->render->camera.y + (App->render->camera.h * SCREEN_SIZE)) {
		bgRect[1].y = bgRect[0].y - bgRect[0].h;
	}
	

	return Update_Status::UPDATE_CONTINUE;
}

bool SceneLevel1::CleanUp()
{
	App->player->Disable();
	App->enemies->Disable();
	App->collisions->Disable();
	App->particles->Disable();

	// TODO 5 (old): Remove All Memory Leaks - no solution here guys ;)
	App->textures->Unload(bgTexture);
	App->textures->Unload(App->player->texture);
	bgTexture = nullptr;
	App->player->texture = nullptr;

	return true;
}