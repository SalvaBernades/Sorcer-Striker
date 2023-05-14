#include "SceneIntro.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "ModuleFonts.h"

SceneIntro::SceneIntro(bool startEnabled) : Module(startEnabled)
{

}

SceneIntro::~SceneIntro()
{

}

// Load assets
bool SceneIntro::Start()
{
	LOG("Loading background assets");

	bool ret = true;

	bgTexture = App->textures->Load("Assets/Sprites/cosmicgames.png");
//	App->audio->PlayMusic("Assets/Music/introTitle.ogg", 1.0f);

	char lookupTable[] = { "! @,_./0123456789$;< ?abcdefghijklmnopqrstuvwxyz" };
	introFont = App->fonts->Load("Assets/Fonts/rtype_font3.png", lookupTable, 2);

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	return ret;
}

Update_Status SceneIntro::Update()
{
	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN || App->input->pads->a)
	{
		App->fade->FadeToBlack(this, (Module*)App->sceneMenu, 60);
	}
	if (App->input->keys[SDL_SCANCODE_ESCAPE] == Key_State::KEY_DOWN)
	{
		return Update_Status::UPDATE_STOP;
	}

	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background
Update_Status SceneIntro::PostUpdate()
{
	// Draw everything --------------------------------------
	

		// Max 32 chars
	App->fonts->BlitText(4, 3, introFont, "cosmicgames");
	App->fonts->BlitText(4, 13, introFont, "sorcerstriker_v0.5.zip");

	App->fonts->BlitText(4, 40, introFont, "upc citm barcelona_video game");
	App->fonts->BlitText(4, 50, introFont, "design and development");
	App->fonts->BlitText(4, 60, introFont, "project1_game prototype");
	App->fonts->BlitText(4, 70, introFont, "jesus alonso alonso");
	App->fonts->BlitText(4, 80, introFont, "marc torres jimenez");

	App->fonts->BlitText(4, 110, introFont, "this project is about the ");
	App->fonts->BlitText(4, 120, introFont, "realization of the pre_alpha");
	App->fonts->BlitText(4, 130, introFont, "of the game sorcer stricker");
	App->fonts->BlitText(4, 140, introFont, "made by the following group");
	App->fonts->BlitText(4, 150, introFont, "of first year students.");

	App->fonts->BlitText(4, 180, introFont, "team members");
	App->fonts->BlitText(4, 190, introFont, "joan marques besses");
	App->fonts->BlitText(4, 200, introFont, "andrea dona roca");
	App->fonts->BlitText(4, 210, introFont, "justo tiscornia sierra");
	App->fonts->BlitText(4, 220, introFont, "salvador bernades iglesias");
	App->fonts->BlitText(4, 230, introFont, "biel linan garcia");

	App->render->Blit(bgTexture, 0, 380, NULL);

	App->fonts->BlitText(4, 490, introFont, "github.com/joanmarquesbesses");
	App->fonts->BlitText(4, 500, introFont, "/sorcer_striker");

	return Update_Status::UPDATE_CONTINUE;
}

bool SceneIntro::CleanUp()
{
	// unload font
	App->textures->Unload(bgTexture);
	bgTexture = nullptr;
	return true;
}
