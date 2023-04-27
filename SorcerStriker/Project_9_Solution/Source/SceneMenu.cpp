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

	bgTexture = App->textures->Load("Assets/Sprites/Background-inicio.png");
	//App->audio->PlayMusic("Assets/Music/stage1.ogg", 1.0f);

	//Bottomside collider
	App->render->camera.x = 0;
	App->render->camera.y = 0;

	return ret;
}

Update_Status SceneMenu::Update()
{
	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		App->fade->FadeToBlack(this, (Module*)App->sceneLevel_1, 120);
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


	App->render->Blit(bgTexture, 0, bgRect.y, NULL);


	return Update_Status::UPDATE_CONTINUE;
}

bool SceneMenu::CleanUp()
{

	// TODO 5 (old): Remove All Memory Leaks - no solution here guys ;)
	App->textures->Unload(bgTexture);
	bgTexture = nullptr;

	return true;
}