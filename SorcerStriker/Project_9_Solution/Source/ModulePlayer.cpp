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
	goingleftAnim.speed = 0.03f;

	// Moving right
	goingrightAnim.PushBack({ 54, 0, 32, 42 });
	goingrightAnim.PushBack({ 100, 0, 31, 42 });
	goingrightAnim.loop = false;
	goingrightAnim.speed = 0.03f;

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
	rollAnim.speed = 0.09f;
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
	delay.setTimer(300);
	autowin = false;
	autolose = false;

	texture = App->textures->Load("Assets/Sprites/naves_bien_4.png");
	winTexture = App->textures->Load("Assets/Sprites/Stage_clear.png");
	looseTexture = App->textures->Load("Assets/Sprites/Game_over.png");
	currentAnimation = &idleAnim;

	laserFx = App->audio->LoadFx("Assets/Fx/laser.wav");
	explosionFx = App->audio->LoadFx("Assets/Fx/Explotas y te mueres.wav");
	roundclear = App->audio->LoadFx("Assets/Fx/roundclrear.wav");
	gameover = App->audio->LoadFx("Assets/Fx/gameover.wav");

	//position.y = (App->render->camera.y / SCREEN_SIZE) + App->render->camera.h - idleAnim.GetCurrentFrame().h;
	//position.x = (App->render->camera.x / SCREEN_SIZE) + (App->render->camera.w / 2) - (idleAnim.GetCurrentFrame().w / 2);
	position.y = (App->render->camera.y / SCREEN_SIZE) + App->render->camera.h - idleAnim.GetCurrentFrame().h;
	position.x = 60;

	lives = 3;
	score = 0;
	canMove = false;
	destroyed = false;

	collider = App->collisions->AddCollider({ position.x, position.y, 39, 42 }, Collider::Type::PLAYER, this);

	// TODO 0: Notice how a font is loaded and the meaning of all its arguments 
	//char lookupTable[] = { "!  ,_./0123456789$;<&?abcdefghijklmnopqrstuvwxyz" };
	//scoreFont = App->fonts->Load("Assets/Fonts/rtype_font.png", "! @,_./0123456789$;<&?abcdefghijklmnopqrstuvwxyz", 1);

	// TODO 4: Try loading "rtype_font3.png" that has two rows to test if all calculations are correct
	char lookupTable[] = { "! @,_./0123456789$;< ?abcdefghijklmnopqrstuvwxyz" };
	scoreFont = App->fonts->Load("Assets/Fonts/rtype_font3.png", lookupTable, 2);

	App->particles->Enable();

	return ret;
}

Update_Status ModulePlayer::Update()
{
	// Moving the player with the camera scroll
	App->player->position.y -= 1;
	if (App->player->position.y < -1500) {
		canMove = true;
	}

	delay.refreshTimer();
	
	if (hit) {
		if (rollAnim.HasFinished()) {
			hit = !hit;
			rollAnim.Reset();
			// posar a la part inferior de la pantalla
 			position.y = (App->render->camera.y / SCREEN_SIZE) + App->render->camera.h - idleAnim.GetCurrentFrame().h;
			position.x = (App->render->camera.x / SCREEN_SIZE) + (App->render->camera.w/2) - (idleAnim.GetCurrentFrame().w/2);
			godMode = false;
			if (lives <= 0) {
				destroyed = true;
				collider->pendingToDelete = true;
				collider = nullptr;
				autolose = true;
				App->audio->PlayFx(gameover, 0);
				App->audio->stopMusic();
				changeScene.resetTimer();
			}
		}
	}
	else if(canMove){
		if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_REPEAT)
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

		if (App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_REPEAT)
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

		if (App->input->keys[SDL_SCANCODE_DOWN] == Key_State::KEY_REPEAT)
		{
			if ((position.y + idleAnim.GetCurrentFrame().h) * SCREEN_SIZE < App->render->camera.y + (App->render->camera.h * SCREEN_SIZE)) {
				position.y += (speed+1);
			}
		}

		if (App->input->keys[SDL_SCANCODE_UP] == Key_State::KEY_REPEAT)
		{
			if ((position.y * SCREEN_SIZE) > App->render->camera.y) {
				position.y -= (speed+1);
			}
		}

		if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
		{
			if (!destroyed && !autowin && delay.hasCompleted()) {
				Particle* newParticle = App->particles->AddParticle(App->particles->laser, position.x + 17, position.y, Collider::Type::PLAYER_SHOT);
				newParticle->collider->AddListener(this);
				App->boost->Shoot();
				App->audio->PlayFx(laserFx);
				delay.resetTimer();
			}
		}

		// If no up/down movement detected, set the current animation back to idle
		if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_IDLE
			&& App->input->keys[SDL_SCANCODE_RIGHT] == Key_State::KEY_IDLE)
			currentAnimation = &idleAnim;
	}

	// If F2 is pressed godMode gets activated
	if (App->input->keys[SDL_SCANCODE_F2] == KEY_DOWN)
		godMode = !godMode;

	if (App->input->keys[SDL_SCANCODE_F3] == KEY_DOWN) {
		if (!autowin && !autolose) {
			autowin = true;
			collider->pendingToDelete = true;
			collider = nullptr;
			App->boost->CleanUp();
			App->audio->PlayFx(roundclear, 0);
			App->audio->stopMusic();
			changeScene.resetTimer();
		}
	}


	if (App->input->keys[SDL_SCANCODE_F4] == KEY_DOWN) {
		if (!autowin && !autolose) {
			autolose = true;
			destroyed = true;
			collider->pendingToDelete = true;
			collider = nullptr;
			App->boost->CleanUp();
			App->audio->PlayFx(gameover, 0);
			App->audio->stopMusic();
			changeScene.resetTimer();
		}
	}

	if (collider != nullptr) {
		collider->SetPos(position.x, position.y);
	}

	currentAnimation->Update();

	// pRESS esc TO QUIT
	if (App->input->keys[SDL_SCANCODE_ESCAPE] == Key_State::KEY_DOWN)
	{
		App->fonts->UnLoad(scoreFont);
		destroyed = true;
		App->boost->CleanUp();
		App->fade->FadeToBlack((Module*)App->sceneLevel_1, (Module*)App->sceneMenu, 120);
	}

	if (autowin || autolose) {
		changeScene.refreshTimer();
		if (changeScene.hasCompleted()) {
			App->fade->FadeToBlack((Module*)App->sceneLevel_1, (Module*)App->sceneMenu, 120);
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
	App->fonts->BlitText(25, 3, scoreFont, scoreText);

	App->fonts->BlitText(5, 13, scoreFont, "lives");
	App->fonts->BlitText(25, 13, scoreFont, livesText);

	return Update_Status::UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 == collider && destroyed == false && !godMode)
	{
		if (c2->type != Collider::Type::BOOST) {
			App->particles->AddParticle(App->particles->explosion, position.x, position.y, Collider::Type::NONE, 9);

			App->audio->PlayFx(explosionFx);
			lives--;
			currentAnimation = &rollAnim;
			hit = true;
			godMode = true;
			App->boost->CleanUp();
		}
	}

	if (c1->type == Collider::Type::PLAYER_SHOT && c2->type == Collider::Type::ENEMY)
	{
		score += 23;
	}
}