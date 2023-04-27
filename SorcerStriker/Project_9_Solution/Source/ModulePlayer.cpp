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

#include <stdio.h>

ModulePlayer::ModulePlayer(bool startEnabled) : Module(startEnabled)
{
	// idle animation - just one sprite
	idleAnim.PushBack({ 0, 0, 65, 66 });
	idleAnim.PushBack({ 75, 0, 65, 66 });
	idleAnim.speed = 0.3f;

	// moving left
	goingleftAnim.PushBack({ 112, 67, 53, 66 });
	goingleftAnim.PushBack({ 58, 67, 53, 66 });
	goingleftAnim.PushBack({ 0, 67, 53, 66 });
	goingleftAnim.loop = false;
	goingleftAnim.speed = 0.1f;

	// left
	leftAnim.PushBack({ 115, 67, 53, 66 });
	leftAnim.PushBack({ 168, 67, 53, 66 });
	leftAnim.speed = 0.03f;

	// Moving right
	goingrightAnim.PushBack({ 0, 134, 53, 66 });
	goingrightAnim.PushBack({ 58, 134, 53, 66 });
	goingrightAnim.PushBack({ 112, 134, 53, 66 });
	goingrightAnim.loop = false;
	goingrightAnim.speed = 0.1f;

	// Right
	rightAnim.PushBack({ 115, 134, 53, 66 });
	rightAnim.PushBack({ 168, 134, 53, 66 });
	rightAnim.speed = 0.3f;

}

ModulePlayer::~ModulePlayer()
{

}

bool ModulePlayer::Start()
{
	LOG("Loading player textures");

	bool ret = true;

	texture = App->textures->Load("Assets/Sprites/naves2.png");
	currentAnimation = &idleAnim;

	//laserFx = App->audio->LoadFx("Assets/Fx/laser.wav");
	//explosionFx = App->audio->LoadFx("Assets/Fx/explosion.wav");

	position.x = 150;
	position.y = 120;

	lives = 3;

	destroyed = false;

	collider = App->collisions->AddCollider({ position.x, position.y, 65, 66 }, Collider::Type::PLAYER, this);

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

	if (App->input->keys[SDL_SCANCODE_A] == Key_State::KEY_REPEAT)
	{
		if ((position.x * SCREEN_SIZE) > App->render->camera.x) {
			position.x -= speed;
		}

		if (currentAnimation != &goingleftAnim)
		{
			goingleftAnim.Reset();
			currentAnimation = &goingleftAnim;
		}
		if (goingleftAnim.HasFinished()) {
			goingleftAnim.Reset();
			currentAnimation = &leftAnim;
		}
	}

	if (App->input->keys[SDL_SCANCODE_D] == Key_State::KEY_REPEAT)
	{
		if ((position.x + idleAnim.GetCurrentFrame().w) * SCREEN_SIZE < (App->render->camera.w * SCREEN_SIZE)) {
			position.x += speed;
		}

		if (currentAnimation != &goingrightAnim && currentAnimation != &rightAnim)
		{
			goingrightAnim.Reset();
			currentAnimation = &goingrightAnim;
		}
		if (goingrightAnim.HasFinished()) {
			goingrightAnim.Reset();
			currentAnimation = &rightAnim;
		}
	}

	if (App->input->keys[SDL_SCANCODE_S] == Key_State::KEY_REPEAT)
	{
		if ((position.y + idleAnim.GetCurrentFrame().h) * SCREEN_SIZE < App->render->camera.y + (App->render->camera.h * SCREEN_SIZE)) {
			position.y += speed;
		}
	}

	if (App->input->keys[SDL_SCANCODE_W] == Key_State::KEY_REPEAT)
	{
		if ((position.y * SCREEN_SIZE) > App->render->camera.y) {
			position.y -= speed;
		}
	}

	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		Particle* newParticle = App->particles->AddParticle(App->particles->laser, position.x + 20, position.y, Collider::Type::PLAYER_SHOT);
		newParticle->collider->AddListener(this);
		//App->audio->PlayFx(laserFx);
	}

	// If no up/down movement detected, set the current animation back to idle
	if (App->input->keys[SDL_SCANCODE_A] == Key_State::KEY_IDLE
		&& App->input->keys[SDL_SCANCODE_D] == Key_State::KEY_IDLE)
		currentAnimation = &idleAnim;

	// If F2 is pressed godMode gets activated
	if (App->input->keys[SDL_SCANCODE_F2] == KEY_DOWN)
		godMode = !godMode;

	collider->SetPos(position.x, position.y);

	currentAnimation->Update();

	// pRESS esc TO QUIT
	if (App->input->keys[SDL_SCANCODE_ESCAPE] == Key_State::KEY_DOWN)
	{
		App->fade->FadeToBlack((Module*)App->sceneLevel_1, (Module*)App->sceneMenu, 120);
		App->fonts->UnLoad(scoreFont);
		destroyed = true;
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

	// Draw UI (score) --------------------------------------
	sprintf_s(scoreText, 10, "%7d", score);
	sprintf_s(livesText, 10, "%7d", lives);

	// TODO 3: Blit the text of the score in at the bottom of the screen
	App->fonts->BlitText(0, 0, scoreFont, scoreText);

	App->fonts->BlitText(150, 248, scoreFont, livesText);

	return Update_Status::UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 == collider && destroyed == false && !godMode)
	{
		App->particles->AddParticle(App->particles->explosion, position.x, position.y, Collider::Type::NONE, 9);
		App->particles->AddParticle(App->particles->explosion, position.x + 8, position.y + 11, Collider::Type::NONE, 14);
		App->particles->AddParticle(App->particles->explosion, position.x - 7, position.y + 12, Collider::Type::NONE, 40);
		App->particles->AddParticle(App->particles->explosion, position.x + 5, position.y - 5, Collider::Type::NONE, 28);
		App->particles->AddParticle(App->particles->explosion, position.x - 4, position.y - 4, Collider::Type::NONE, 21);

		//App->audio->PlayFx(explosionFx);
		lives--;
		if (lives <= 0) {
			App->fade->FadeToBlack((Module*)App->sceneLevel_1, (Module*)App->sceneMenu, 120);
			App->fonts->UnLoad(scoreFont);
			destroyed = true;
		}
		
	}

	if (c1->type == Collider::Type::PLAYER_SHOT && c2->type == Collider::Type::ENEMY)
	{
		score += 23;
	}
}