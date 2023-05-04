#include "SceneLevel1.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleCollisions.h"
#include "ModuleEnemies.h"
#include "ModulePlayer.h"
#include "ModuleParticles.h"
#include "ModuleBoost.h"

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

	bgTextureIntro = App->textures->Load("Assets/Sprites/Background-stage1-2.png");
	bgTextureForest = App->textures->Load("Assets/Sprites/Background-forest.png");
	bgTextureFirst = App->textures->Load("Assets/Sprites/Background-stage1-1.png");
	bgTextureLast = App->textures->Load("Assets/Sprites/Background-stage1-3.png");
	App->audio->PlayMusic("Assets/Music/stage1.ogg", 1.0f);
	uint w, h;
	bgRect[0].x = 0;
	bgRect[0].y = -50;
	App->textures->GetTextureSize(bgTextureIntro, w, h);
	bgRect[1].x = 0;
	bgRect[1].y = bgRect[0].y - h;
	bgRect[0].w = w;
	bgRect[0].h = h;
	bgRect[1].w = w;
	bgRect[1].h = h;

	changeBG = 0;

//Enemies ---

	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 0, -2200);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 50, -2200);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 100, -2200);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 150, -2200);
	
	
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 75, -2600);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 125, -2600);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 175, -2600);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 225, -2600);
	
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 0, -3000);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 50, -3000);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 100, -3000);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 150, -3000);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -3000);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 250, -3000);

	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 100,-2750);
	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 150, -2750);

	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 0, -3150);
	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 50, -3150);

	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 150, -3150);
	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 200, -3150);

	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 175, -3700);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 225, -3700);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 0, -3700);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 50, -3700);

	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 0, -4000);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 50, -4000);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 100, -4000);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 150, -4000);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -4000);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 250, -4000);

	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 0, -4300);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 50, -4300);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 100, -4300);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 150, -4300);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -4300);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 250, -4300);

	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 100, -4250);
	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 150, -4250);

	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 0, -4150);
	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 50, -4150);

	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 150, -4150);
	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 200, -4150);



	App->boost->AddBoost(Boost_Type::LASERFIST, 120, -1800);//-1800

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	App->enemies->Enable();
	App->collisions->Enable();
	App->boost->Enable();
	App->player->Enable();


	return ret;
}

Update_Status SceneLevel1::Update()
{
	App->render->camera.y -= 2;

	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background
Update_Status SceneLevel1::PostUpdate()
{
	// Draw everything --------------------------------------
	//App->render->Blit(bgTexture, 0, 0, NULL);

	if (App->player->autowin) {
		App->enemies->Disable();
	}


	if (bgRect[0].y * SCREEN_SIZE > App->render->camera.y + (App->render->camera.h * SCREEN_SIZE)) {
		bgRect[0].y = bgRect[1].y - bgRect[1].h;
		changeBG++;
	}
	else if (bgRect[1].y * SCREEN_SIZE > App->render->camera.y + (App->render->camera.h * SCREEN_SIZE)) {
		bgRect[1].y = bgRect[0].y - bgRect[0].h;
		changeBG++;
	}
	
	if (changeBG == 0) {
		App->render->Blit(bgTextureFirst, 0, bgRect[0].y, NULL);
		App->render->Blit(bgTextureIntro, 0, bgRect[1].y, NULL);
	}
	else if(changeBG < 2){
		App->render->Blit(bgTextureIntro, 0, bgRect[0].y, NULL);
		App->render->Blit(bgTextureIntro, 0, bgRect[1].y, NULL);
	}
	else if(changeBG < 3) {
		App->render->Blit(bgTextureIntro, 0, bgRect[0].y, NULL);
		App->render->Blit(bgTextureForest, 0, bgRect[1].y, NULL);
		App->render->Blit(bgTextureLast, 0, bgRect[1].y, NULL);
	}
	else if (changeBG < 4) {
		App->render->Blit(bgTextureForest, 0, bgRect[0].y, NULL);
		App->render->Blit(bgTextureForest, 0, bgRect[1].y, NULL);
		App->render->Blit(bgTextureLast, 0, bgRect[1].y, NULL);
	}
	else {
		App->render->Blit(bgTextureForest, 0, bgRect[0].y, NULL);
		App->render->Blit(bgTextureForest, 0, bgRect[1].y, NULL);
	}

	return Update_Status::UPDATE_CONTINUE;
}

bool SceneLevel1::CleanUp()
{
	App->player->Disable();
	App->enemies->Disable();
	App->collisions->Disable();
 	App->particles->Disable();
	App->boost->Disable();

	// TODO 5 (old): Remove All Memory Leaks - no solution here guys ;)
	App->textures->Unload(bgTextureIntro);
	App->textures->Unload(bgTextureForest);
	App->textures->Unload(bgTextureFirst);
	App->textures->Unload(bgTextureLast);
	App->textures->Unload(App->player->texture);
	bgTextureIntro = nullptr;
	App->player->texture = nullptr;

	return true;
}