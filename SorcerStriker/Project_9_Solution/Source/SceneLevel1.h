#ifndef __SCENE_LEVEL1_H__
#define __SCENE_LEVEL1_H__

#include "Module.h"
#include "Animation.h"
#include "Timer.h"


enum class Scene_States
{
	INTRO,
	FOREST,
	SEA,
	CASTLE,
};

struct SDL_Texture;

class SceneLevel1 : public Module
{
public:
	//Constructor
	SceneLevel1(bool startEnabled);

	//Destructor
	~SceneLevel1();

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start() override;

	// Called at the middle of the application loop
	// Updates the scene's background animations
	Update_Status Update() override;

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	Update_Status PostUpdate() override;

	// Disables the player and the enemies
	bool CleanUp();

public:
	
	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* bgTextureIntro = nullptr;
	SDL_Texture* bgTextureForest = nullptr;
	SDL_Texture* bgTextureFirst = nullptr;
	SDL_Texture* bgTextureLast = nullptr;
	SDL_Texture* bgTexturePlaya = nullptr;
	SDL_Texture* bgTextureMar = nullptr;
	SDL_Texture* bgTextureCastle1 = nullptr;
	SDL_Texture* bgTextureCastle2 = nullptr;
	SDL_Texture* bgTextureCastle3 = nullptr;
	SDL_Texture* bgTextureFinalCastle1 = nullptr;
	SDL_Texture* bgTextureFinalCastle2 = nullptr;
	SDL_Texture* bgTextureFinalCastle3 = nullptr;
	SDL_Texture* bgTextureFinalCastle4 = nullptr;
	SDL_Texture* bgTextureFinal = nullptr;
	SDL_Rect bgRect[2];
	SDL_Rect bgRectAux[2];

	int changeBG;
	int bosswarning = 0;
	bool bossm = false;
	bool bosssf = false;

	Scene_States state;
	bool start;
	Timer sceneIntro;
};

#endif