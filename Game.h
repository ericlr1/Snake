#pragma once

#include "SDL/include/SDL.h"
#include "SDL_image/include/SDL_image.h"
#include "SDL_mixer/include/SDL_mixer.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

#include "Entity.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WINDOW_WIDTH	1024
#define WINDOW_HEIGHT	768
#define MAX_KEYS		256
#define MAX_METEORS		300
#define MAX_SHOTS		32
#define MAX_TRACKS		8

class Game
{
public:
	Game();
	~Game();

	bool Init();
	bool LoadImages();
	bool LoadAudios();
	void Release();
	
	bool Input();
	bool Update();
	void Draw();

	

private:
	void DrawCollider();
	void CheckCollider();
	void UpdateGame();
	void StartGame();

	
public:
	bool finish = false;

private:
	SDL_Window *Window;
	SDL_Renderer *Renderer;
	SDL_Texture *img_background, *img_player, *img_obj, *img_can, *img_menu, *img_over;  //Incluido el *img_menu

	Entity Player, Meteor, Candy, Scene, Menu;
	int idx_obj;
	
	Entity ColliderTest;
	
	bool god_mode;

	enum KEY_STATE { KEY_IDLE, KEY_DOWN, KEY_REPEAT, KEY_UP	};
	KEY_STATE keys[MAX_KEYS]; 

	int current_time = 0;
	int start_meteor_time = 0;
	
	bool firstime_showing_game = true;

	bool restart_meteor = true;
	Mix_Music* tracks[MAX_TRACKS];
	int num_tracks;

	Mix_Chunk* sfxs[MAX_SHOTS];
	int num_sfxs;
	
	
};
