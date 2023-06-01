#include "ModulePlayer.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModuleCollisions.h"
#include "ModuleFadeToBlack.h"
#include "ModuleFonts.h"
#include "ModuleBoost.h"

#include <stdio.h>

ModulePlayer::ModulePlayer(bool startEnabled) : Module(startEnabled)
{
	// idle animation - just one sprite
	idleAnim.PushBack({ 0, 0, 39, 42 });

	// moving left
	goingleftAnim.PushBack({ 419, 0, 30, 42 });
	goingleftAnim.PushBack({ 374, 0, 30, 42 });
	goingleftAnim.loop = false;
	goingleftAnim.speed = 0.1f;

	// Moving right
	goingrightAnim.PushBack({ 54, 0, 32, 42 });
	goingrightAnim.PushBack({ 100, 0, 31, 42 });
	goingrightAnim.loop = false;
	goingrightAnim.speed = 0.1f;

	// rolling left
	rollAnim.PushBack({ 0, 0, 39, 42 });
	rollAnim.PushBack({ 419, 0, 30, 42 });
	rollAnim.PushBack({ 374, 0, 30, 42 });
	rollAnim.PushBack({ 332, 0, 28, 42 });
	rollAnim.PushBack({ 288, 0, 32, 42 });
	rollAnim.PushBack({ 231, 0, 43, 42 });
	rollAnim.PushBack({ 187, 0, 30, 42 });
	rollAnim.PushBack({ 145, 0, 28, 42 });
	rollAnim.PushBack({ 100, 0, 31, 42 });
	rollAnim.PushBack({ 54, 0, 32, 42 });
	rollAnim.PushBack({ 0, 0, 39, 42 });
	rollAnim.PushBack({ 419, 0, 30, 42 });
	rollAnim.PushBack({ 374, 0, 30, 42 });
	rollAnim.speed = 0.3f;
	rollAnim.loop = false;

}

ModulePlayer::~ModulePlayer()
{

}

bool ModulePlayer::Start()
{
	LOG("Loading player textures");

	bool ret = true;
	changeScene.resetTimer();
	changeScene.setTimer(7000);
	shootdelay.setTimer(350);
	spawnInvul.setTimer(2000);
	autowin = false;
	autolose = false;
	godMode = false;
	State = Player_States::INTRO;

	texture = App->textures->Load("Assets/Sprites/naves_bien_4.png");
	winTexture = App->textures->Load("Assets/Sprites/Stage_clear.png");
	looseTexture = App->textures->Load("Assets/Sprites/Game_over.png");
	currentAnimation = &idleAnim;

	laserFx = App->audio->LoadFx("Assets/Fx/laser.wav");
	explosionFx = App->audio->LoadFx("Assets/Fx/Explotas y te mueres.wav");
	roundclear = App->audio->LoadFx("Assets/Fx/roundclrear.wav");
	gameover = App->audio->LoadFx("Assets/Fx/gameover.wav");

	position.y = (App->render->camera.y) + App->render->camera.h - idleAnim.GetCurrentFrame().h - 30;
	position.x = 60;

	lives = 3;
	score = 000;
	canMove = false;
	destroyed = false;
	speed = 4;
	playerWidth = 39;
	playerHeigth = 42;

	activeBoost = nullptr;
	collider = App->collisions->AddCollider({ position.x + 9, position.y + 6, 21, 30 }, Collider::Type::PLAYER, this);

	char lookupTable[] = { "! @,_./0123456789$;< ?abcdefghijklmnopqrstuvwxyz" };
	scoreFont = App->fonts->Load("Assets/Fonts/rtype_font3.png", lookupTable, 2);

	App->particles->Enable();

	return ret;
}

Update_Status ModulePlayer::Update()
{
	App->player->position.y -= App->render->cameraSpeed;
	shootdelay.refreshTimer();

	switch (State)
	{
	case Player_States::INTRO:

		if (App->player->position.y < -1300) {
			State = Player_States::PLAYING;
			App->render->cameraSpeed = 5;
		}
		break;

	case Player_States::PLAYING:

		if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_REPEAT || App->input->pads->l_x < 0)
		{
			if ((position.x * SCREEN_SIZE) > App->render->camera.x) {
				position.x -= speed;
			}

			if (currentAnimation != &goingleftAnim)
			{
				goingleftAnim.Reset();
				currentAnimation = &goingleftAnim;
			}
		}

		if (App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_REPEAT || App->input->pads->l_x > 0)
		{
			if ((position.x + idleAnim.GetCurrentFrame().w) * SCREEN_SIZE < (App->render->camera.w * SCREEN_SIZE)) {
				position.x += speed;
			}

			if (currentAnimation != &goingrightAnim)
			{
				goingrightAnim.Reset();
				currentAnimation = &goingrightAnim;
			}
		}

		if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_REPEAT || App->input->pads->l_y > 0)
		{
			if ((position.y + idleAnim.GetCurrentFrame().h) * SCREEN_SIZE < App->render->camera.y + (App->render->camera.h * SCREEN_SIZE)) {
				position.y += speed;
			}
		}

		if (App->input->keys[SDL_SCANCODE_UP] == Key_State::KEY_REPEAT || App->input->pads->l_y < 0)
		{
			if ((position.y * SCREEN_SIZE) > App->render->camera.y) {
				position.y -= speed;
			}
		}

		if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN || App->input->pads->a)
		{
			if (shootdelay.hasCompleted()) {
				Particle* newParticle = App->particles->AddParticle(App->particles->laser, position.x + 12, position.y, Collider::Type::PLAYER_SHOT);
				newParticle->collider->AddListener(this);
				newParticle = App->particles->AddParticle(App->particles->laser, position.x + 22, position.y, Collider::Type::PLAYER_SHOT);
				newParticle->collider->AddListener(this);
				newParticle = nullptr;
				if (activeBoost != nullptr) {
					activeBoost->Shoot();
				}
				App->audio->PlayFx(laserFx);
				shootdelay.resetTimer();
			}
		}

		// If no up/down movement detected, set the current animation back to idle
		if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_IDLE
			&& App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_IDLE)
			currentAnimation = &idleAnim;

		if (hit) {
			spawnInvul.refreshTimer();
			if (spawnInvul.hasCompleted()) {
				hit = false;
				spawnInvul.resetTimer();
			}
		}

		break;

	case Player_States::HIT:

		if (rollAnim.HasFinished()) {
			rollAnim.Reset();
			spawnInvul.resetTimer();
			// posar a la part inferior de la pantalla
			position.y = (App->render->camera.y) + App->render->camera.h - idleAnim.GetCurrentFrame().h;
			position.x = (App->render->camera.x) + (App->render->camera.w / 2) - (idleAnim.GetCurrentFrame().w / 2);
			State = Player_States::PLAYING;
			if (lives <= 0) {
				autolose = true;
				State = Player_States::DESTROYED;
				App->audio->PlayFx(gameover, 0);
				destroyed = true;
				changeScene.resetTimer();
			}
		}
		break;

	case Player_States::DESTROYED:

		if (collider != nullptr) {
			collider->pendingToDelete = true;
			collider = nullptr;
			App->boost->CleanUp();
			App->audio->stopMusic();
		}
		changeScene.refreshTimer();
		if (changeScene.hasCompleted()) {
			destroyed = true;
			App->fade->FadeToBlack((Module*)App->sceneLevel_1, (Module*)App->sceneMenu, 60);
		}
		break;

	default:
		break;
	}

	if (App->input->keys[SDL_SCANCODE_F2] == KEY_DOWN)
		godMode = !godMode;

	if (App->input->keys[SDL_SCANCODE_F3] == KEY_DOWN) {
		if (!autowin && !autolose) {
			changeScene.resetTimer();
			autowin = true;
			App->audio->PlayFx(roundclear, 0);
			State = Player_States::DESTROYED;
		}
	}

	if (App->input->keys[SDL_SCANCODE_F4] == KEY_DOWN) {
		if (!autowin && !autolose) {
			changeScene.resetTimer();
			autolose = true;
			destroyed = true;
			App->audio->PlayFx(gameover, 0);
			State = Player_States::DESTROYED;
		}
	}

	if (collider != nullptr) {
		collider->SetPos(position.x + 9, position.y + 6);
	}

	currentAnimation->Update();

	// PRESS esc TO QUIT
	if (App->input->keys[SDL_SCANCODE_ESCAPE] == Key_State::KEY_DOWN)
	{
		if (collider != nullptr) {
			collider->pendingToDelete = true;
			collider = nullptr;
			App->boost->CleanUp();
			App->audio->stopMusic();
			destroyed = true;
			App->fade->FadeToBlack((Module*)App->sceneLevel_1, (Module*)App->sceneMenu, 60);
		}
	}

	return Update_Status::UPDATE_CONTINUE;
}

Update_Status ModulePlayer::PostUpdate()
{
	if (!destroyed)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		App->render->Blit(texture, position.x, position.y, &rect);
	}
	
	if (autolose) {
		App->render->Blit(looseTexture, 58, 80, nullptr, 0.0f ,false);
	}

	if (autowin) {
		App->render->Blit(winTexture, 40, 80, nullptr, 0.0f, false);
	}

	// Draw UI (score) --------------------------------------
	sprintf_s(scoreText, 10, "%7d", score);
	sprintf_s(livesText, 10, "%7d", lives);

	// TODO 3: Blit the text of the score in at the bottom of the screen
	App->fonts->BlitText(5, 3, scoreFont, "score");
	App->fonts->BlitText(35, 3, scoreFont, scoreText);

	App->fonts->BlitText(5, 13, scoreFont, "lives");
	App->fonts->BlitText(35, 13, scoreFont, livesText);

	return Update_Status::UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 == collider && destroyed == false && !godMode && !hit)
	{
		if (c2->type != Collider::Type::BOOST) {
			App->particles->AddParticle(App->particles->explosion, position.x, position.y, Collider::Type::NONE);

			App->audio->PlayFx(explosionFx);
			lives--;
			currentAnimation = &rollAnim;
			hit = true;
			if (activeBoost != nullptr) {
				activeBoost->destroyed = true;
				activeBoost->pendingToDelete = true;
				activeBoost->taken = false;
				activeBoost = nullptr;
			}
			State = Player_States::HIT;
		}
	}
}

bool ModulePlayer::CleanUp()
{
	if (collider != nullptr) {
		collider->pendingToDelete = true;
		collider = nullptr;
	}
	App->textures->Unload(texture);
	App->textures->Unload(winTexture);
	App->textures->Unload(looseTexture);
	App->fonts->UnLoad(scoreFont);
	App->audio->Unload(laserFx);
	App->audio->Unload(explosionFx);
	App->audio->Unload(roundclear);
	App->audio->Unload(gameover);
	texture = nullptr;
	winTexture = nullptr;
	looseTexture = nullptr;
	return true;
}
