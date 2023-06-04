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
#include "ModuleInput.h"

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
	bgTexturePlaya = App->textures->Load("Assets/Sprites/playa.png");
	bgTextureMar = App->textures->Load("Assets/Sprites/Background-water.png");
	bgTextureCastle1 = App->textures->Load("Assets/Sprites/Background-floatingcastle-1.png");
	bgTextureCastle2 = App->textures->Load("Assets/Sprites/Background-floatingcastle-2.png");
	bgTextureCastle3 = App->textures->Load("Assets/Sprites/Background-floatingcastle-3.png");
	bgTextureFinalCastle1 = App->textures->Load("Assets/Sprites/afterCastle1.png");
	bgTextureFinalCastle2 = App->textures->Load("Assets/Sprites/afterCastle2.png");
	bgTextureFinalCastle3 = App->textures->Load("Assets/Sprites/afterCastle3.png");
	bgTextureFinalCastle4 = App->textures->Load("Assets/Sprites/afterCastle4.png");
	bgTextureFinal = App->textures->Load("Assets/Sprites/finalCastle.png");
	App->audio->PlayMusic("Assets/Music/stage1.ogg", 1.0f);
	uint w, h;
	bgRect[0].x = 0;
	bgRect[0].y = 75;
	App->textures->GetTextureSize(bgTextureIntro, w, h);
	bgRect[1].x = 0;
	bgRect[1].y = bgRect[0].y - h;
	bgRect[0].w = w;
	bgRect[0].h = h;
	bgRect[1].w = w;
	bgRect[1].h = h;
	bosswarning = App->audio->LoadFx("Assets/Fx/WARNING JEFE DRAGON.wav");

	changeBG = 0;

//Enemies ---

	// red bird 50 px margin

	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 50, -2200, 0);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 50, -2250, 0);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 50, -2300, 0);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 50, -2350, 0);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 50, -2400, 0);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 50, -2450, 0);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 50, -2500, 0);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 50, -2550, 0);
		
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -2600, 1);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -2650, 1);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -2700, 1);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -2750, 1);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -2800, 1);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -2850, 1);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -2900, 1);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -2950, 1);
	
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 125, -3000, 2);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 125, -3050, 2);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 125, -3100, 2);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 125, -3150, 2);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 125, -3200, 2);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 125, -3250, 2);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 125, -3300, 2);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 125, -3350, 2);

	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -3600, 3);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -3650, 3);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -3700, 3);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -3750, 3);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -3800, 3);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -3850, 3);
	
	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 50, -3700, 0);
	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 100, -3700, 0);

	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 0, -4000, 0);
	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 50, -4000, 0);

	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 150, -4500, 0);
	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 200, -4500 , 0);

	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -4600, 1);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -4650, 1);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -4700, 1);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -4750, 1);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -4800, 1);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -4850, 1);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -4900, 1);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -4950, 1);

	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 150, -5000, 0);
	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 200, -5000, 0);

	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 50, -5700, 0);
	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 100, -5700, 0);

	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 150, -6350, 0);
	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 200, -6350, 0);

	App->enemies->AddEnemy(Enemy_Type::MECH, 150, -5500, 0);
	App->enemies->AddEnemy(Enemy_Type::MECH, 5, -6500, 0);

	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -8600, 1);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -8650, 1);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -8700, 1);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -8750, 1);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -8800, 1);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -8850, 1);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -8900, 1);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -8950, 1);

	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 150, -10000, 0);
	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 200, -10000, 0);

	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 50, -11700, 0);
	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 100, -11700, 0);

	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 150, -12350, 0);
	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 200, -12350, 0);

	App->enemies->AddEnemy(Enemy_Type::MECH, 150, -12500, 0);
	App->enemies->AddEnemy(Enemy_Type::MECH, 5, -12500, 0);

	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 150, -13000, 0);
	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 200, -13000, 0);

	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 50, -13700, 0);
	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 100, -13700, 0);

	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 150, -14350, 0);
	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 200, -14350, 0);

	App->enemies->AddEnemy(Enemy_Type::MECH, 150, -14500, 0);
	App->enemies->AddEnemy(Enemy_Type::MECH, 5, -14500, 0);

	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -14600, 1);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -14650, 1);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -14700, 1);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -14750, 1);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -14800, 1);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -14850, 1);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -14900, 1);
	App->enemies->AddEnemy(Enemy_Type::REDBIRD, 200, -14950, 1);

	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 150, -15000, 0);
	App->enemies->AddEnemy(Enemy_Type::BROWNSHIP, 200, -15000, 0);

//-------
	App->boost->AddBoost(Boost_Type::LASERFIST, 120, -1800);//-1800
	App->boost->AddBoost(Boost_Type::COINBAG, 120, -1500);//-1800
	// tank amb 320 queda al limit
	App->enemies->AddEnemy(Enemy_Type::TANK, 320, -9055, 0);//-1800
	App->enemies->AddEnemy(Enemy_Type::TANK, 280, -9055, 0);
	App->enemies->AddEnemy(Enemy_Type::TANK, 240, -9055, 0);

	App->enemies->AddEnemy(Enemy_Type::TANK, -80, -9055, 1);//-1800
	App->enemies->AddEnemy(Enemy_Type::TANK, -40, -9055, 1);
	App->enemies->AddEnemy(Enemy_Type::TANK, 0, -9055, 1);
	//-17000
	App->enemies->AddEnemy(Enemy_Type::FINALBOSS, 0, -17000, 0);

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	App->enemies->Enable();
	App->collisions->Enable();
	App->boost->Enable();
	App->player->Enable();
	start = false;
	App->render->cameraSpeed = 0;

	sceneIntro.setTimer(5000);
	sceneIntro.resetTimer();
	bossm = false;
	bosssf = false;

	state = Scene_States::INTRO;

	return ret;
}

Update_Status SceneLevel1::Update()
{
	App->render->camera.y -= App->render->cameraSpeed;

	if (App->player->position.y < -15500 && !bosssf) {
		App->audio->PlayFx(bosswarning);
		bosssf = true;
	}
	if (App->player->position.y < -16500 && !bossm) {
		bossm = true;
		App->audio->PlayBossMusic();
	}

	sceneIntro.refreshTimer();
	if (sceneIntro.hasCompleted() && !start) {
		App->render->cameraSpeed = 8;
		start = true;
	}

	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN || App->input->pads->a)
	{
		sceneIntro.setTimer(0);
	}

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


	if (bgRect[0].y > App->render->camera.y + App->render->camera.h) {
		bgRect[0].y = bgRect[1].y - bgRect[1].h;
		changeBG++;
	}
	else if (bgRect[1].y > App->render->camera.y + App->render->camera.h) {
		bgRect[1].y = bgRect[0].y - bgRect[0].h;
		changeBG++;
	}

	switch (state)
	{
	case Scene_States::INTRO:
		if (changeBG == 0) {
			App->render->Blit(bgTextureFirst, 0, bgRect[0].y, NULL);
			App->render->Blit(bgTextureIntro, 0, bgRect[1].y, NULL);
		}
		else if (changeBG < 2) {
			App->render->Blit(bgTextureIntro, 0, bgRect[0].y, NULL);
			App->render->Blit(bgTextureIntro, 0, bgRect[1].y, NULL);
		}
		else if (changeBG < 3) {
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
			if (changeBG > 8) {
				state = Scene_States::FOREST;
				changeBG = 0;
			}
		}
		break;
	case Scene_States::FOREST:
		if (changeBG == 0) {
			App->render->Blit(bgTextureForest, 0, bgRect[0].y, NULL);
			App->render->Blit(bgTextureForest, 0, bgRect[1].y, NULL);
		}
		else if (changeBG < 2) {
			App->render->Blit(bgTextureForest, 0, bgRect[0].y, NULL);
			App->render->Blit(bgTexturePlaya, 0, bgRect[1].y, NULL);
		}
		else if (changeBG < 3) {
			App->render->Blit(bgTextureMar, 0, bgRect[0].y, NULL);
			App->render->Blit(bgTexturePlaya, 0, bgRect[1].y, NULL);
		}
		else {
			App->render->Blit(bgTextureMar, 0, bgRect[0].y, NULL);
			App->render->Blit(bgTextureMar, 0, bgRect[1].y, NULL);
			state = Scene_States::SEA;
			changeBG = 0;
		}
		break;
	case Scene_States::SEA:
		if (changeBG < 5) {
			App->render->Blit(bgTextureMar, 0, bgRect[0].y, NULL);
			App->render->Blit(bgTextureMar, 0, bgRect[1].y, NULL);
		}
		else if (changeBG < 6){
			state = Scene_States::CASTLE;
			App->render->cameraSpeed = App->render->cameraSpeed / 2;
			App->render->Blit(bgTextureMar, 0, bgRect[0].y, NULL);
			App->render->Blit(bgTextureCastle1, 0, bgRect[0].y, NULL);
			App->render->Blit(bgTextureMar, 0, bgRect[1].y, NULL);
			changeBG = 0;
			bgRectAux[0] = bgRect[0];
			bgRectAux[1] = bgRect[1];
		}
		break;
	case Scene_States::CASTLE:

		if (changeBG < 3) {
			if (bgRectAux[0].y > App->render->camera.y + App->render->camera.h) {
				bgRectAux[0].y = bgRectAux[1].y - bgRectAux[1].h;
			}
			else if (bgRectAux[1].y > App->render->camera.y + App->render->camera.h) {
				bgRectAux[1].y = bgRectAux[0].y - bgRectAux[0].h;
			}
			App->render->Blit(bgTextureMar, 0, bgRectAux[0].y, NULL);
			App->render->Blit(bgTextureMar, 0, bgRectAux[1].y, NULL);
			bgRectAux[0].y += App->render->cameraSpeed;
			bgRectAux[1].y += App->render->cameraSpeed;
		}

		if (changeBG >= 14) {
			if (bgRectAux[0].y > App->render->camera.y + App->render->camera.h) {
				bgRectAux[0].y = bgRectAux[1].y - bgRectAux[1].h;
			}
			else if (bgRectAux[1].y > App->render->camera.y + App->render->camera.h) {
				bgRectAux[1].y = bgRectAux[0].y - bgRectAux[0].h;
			}
			App->render->Blit(bgTextureForest, 0, bgRectAux[0].y, NULL);
			App->render->Blit(bgTextureForest, 0, bgRectAux[1].y, NULL);
			bgRectAux[0].y += App->render->cameraSpeed;
			bgRectAux[1].y += App->render->cameraSpeed;
		}

		if (changeBG < 1) {
			App->render->Blit(bgTextureCastle1, 0, bgRect[0].y, NULL);
		}
		else if (changeBG < 2) {
			App->render->Blit(bgTextureCastle1, 0, bgRect[0].y, NULL);
			App->render->Blit(bgTextureCastle2, 0, bgRect[1].y, NULL);
		}
		else if (changeBG < 3) {
			App->render->Blit(bgTextureCastle2, 0, bgRect[0].y, NULL);
			App->render->Blit(bgTextureCastle2, 0, bgRect[1].y, NULL);
		}
		else if (changeBG < 4) {
			App->render->Blit(bgTextureCastle2, 0, bgRect[0].y, NULL);
			App->render->Blit(bgTextureCastle3, 0, bgRect[1].y, NULL);
		}
		else if (changeBG < 5) {
			App->render->Blit(bgTextureFinalCastle1, 0, bgRect[0].y, NULL);
			App->render->Blit(bgTextureCastle3, 0, bgRect[1].y, NULL);
		}
		else if (changeBG < 6) {
			App->render->Blit(bgTextureFinalCastle1, 0, bgRect[0].y, NULL);
			App->render->Blit(bgTextureFinalCastle2, 0, bgRect[1].y, NULL);
		}
		else if (changeBG < 8) {
			App->render->Blit(bgTextureFinalCastle2, 0, bgRect[0].y, NULL);
			App->render->Blit(bgTextureFinalCastle2, 0, bgRect[1].y, NULL);
		}
		else if (changeBG < 9) {
			App->render->Blit(bgTextureFinalCastle3, 0, bgRect[0].y, NULL);
			App->render->Blit(bgTextureFinalCastle2, 0, bgRect[1].y, NULL);
		}
		else if (changeBG < 11) {
			App->render->Blit(bgTextureFinalCastle3, 0, bgRect[0].y, NULL);
			App->render->Blit(bgTextureFinalCastle3, 0, bgRect[1].y, NULL);
		}
		else if (changeBG < 12) {
			App->render->Blit(bgTextureFinalCastle3, 0, bgRect[0].y, NULL);
			App->render->Blit(bgTextureFinalCastle4, 0, bgRect[1].y, NULL);
		}
		else if (changeBG < 14) {
			bgRectAux[0] = bgRect[0];
			bgRectAux[1] = bgRect[1];
			App->render->Blit(bgTextureFinalCastle4, 0, bgRect[0].y, NULL);
			App->render->Blit(bgTextureFinalCastle4, 0, bgRect[1].y, NULL);
		}
		else if (changeBG < 15) {
			App->render->Blit(bgTextureFinal, 0, bgRect[0].y, NULL);
			App->render->Blit(bgTextureFinalCastle4, 0, bgRect[1].y, NULL);
		}
		else if (changeBG < 16) {
			App->render->Blit(bgTextureFinal, 0, bgRect[0].y, NULL);
		}
		break;
	default:
		break;
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
	sceneIntro.refreshTimer();

	// TODO 5 (old): Remove All Memory Leaks - no solution here guys ;)
	App->textures->Unload(bgTextureIntro);
	App->textures->Unload(bgTextureForest);
	App->textures->Unload(bgTextureFirst);
	App->textures->Unload(bgTextureLast);
	App->textures->Unload(bgTextureCastle1);
	App->textures->Unload(bgTextureCastle2);
	App->textures->Unload(bgTextureCastle3);
	App->textures->Unload(bgTextureCastle3);
	App->textures->Unload(bgTextureFinalCastle1);
	App->textures->Unload(bgTextureFinalCastle2);
	App->textures->Unload(bgTextureFinalCastle3);
	App->textures->Unload(bgTextureFinalCastle4);
	App->textures->Unload(bgTextureFinal);
	App->textures->Unload(bgTextureMar);
	App->textures->Unload(bgTexturePlaya);
	bgTextureForest = nullptr;
	bgTextureFirst = nullptr;
	bgTextureLast = nullptr;
	bgTextureIntro = nullptr;
	bgTextureCastle1 = nullptr;
	bgTextureCastle2 = nullptr;
	bgTextureCastle3 = nullptr;

	return true;
}