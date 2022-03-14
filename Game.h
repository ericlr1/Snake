#pragma once

#include "SDL/include/SDL.h"
#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )


#include "Entity.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WINDOW_WIDTH	1024
#define WINDOW_HEIGHT	768
#define MAX_KEYS		256
#define MAX_SHOTS		300

class Game
{
public:
	Game();
	~Game();

	bool Init();
	bool LoadImages();
	void Release();
	
	bool Input();
	bool Update();
	void Draw();

private:
	void DrawCollider();
	void CheckCollider();

private:
	SDL_Window *Window;
	SDL_Renderer *Renderer;
	SDL_Texture *img_background, *img_player, *img_obj, *img_menu;  //Incluido el *img_menu

	Entity Player, Object[MAX_SHOTS], Scene, Menu;
	int idx_obj;
	
	Entity ColliderTest;
	
	bool god_mode;

	enum KEY_STATE { KEY_IDLE, KEY_DOWN, KEY_REPEAT, KEY_UP	};
	KEY_STATE keys[MAX_KEYS]; 
};
