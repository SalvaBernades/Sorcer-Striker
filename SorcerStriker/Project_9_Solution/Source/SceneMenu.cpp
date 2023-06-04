#include "SceneMenu.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"

SceneMenu::SceneMenu(bool startEnabled) : Module(startEnabled)
{

}

SceneMenu::~SceneMenu()
{

}

// Load assets
bool SceneMenu::Start()
{
	LOG("Loading background assets");

	bool ret = true;

	bgTexture = App->textures->Load("Assets/Sprites/Background-menu1.png");
	App->audio->PlayMusic("Assets/Music/introTitle.ogg", 1.0f);
	App->textures->GetTextureSize(bgTexture,w, h);
	startFX = App->audio->LoadFx("Assets/Fx/Start game.wav");
	started = false;
	//Bottomside collider
	App->render->camera.x = 0;
	App->render->camera.y = 0;

	return ret;
}

Update_Status SceneMenu::Update()
{
	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN || App->input->pads->a)
	{
		App->fade->FadeToBlack(this, (Module*)App->sceneLevel_1, 60);
		if (!started) {
			App->audio->PlayFx(startFX);
			started = true;
		}
	}
	if (App->input->keys[SDL_SCANCODE_ESCAPE] == Key_State::KEY_DOWN)
	{
		return Update_Status::UPDATE_STOP;
	}

	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background
Update_Status SceneMenu::PostUpdate()
{
	// Draw everything --------------------------------------
	//App->render->Blit(bgTexture, 0, 0, NULL);

	App->render->Blit(bgTexture, 0, (SCREEN_HEIGHT/2) - (h/2), NULL);


	return Update_Status::UPDATE_CONTINUE;
}

bool SceneMenu::CleanUp()
{

	// TODO 5 (old): Remove All Memory Leaks - no solution here guys ;)
	App->textures->Unload(bgTexture);
	bgTexture = nullptr;

	return true;
}