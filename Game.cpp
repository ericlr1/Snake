#include "Game.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>

bool enter = false;
int RandObj = 0;
int vida = 3;

Game::Game() {}
Game::~Game(){}

bool Game::Init()
{
	//Initialize SDL with all subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	//Create our window: title, x, y, w, h, flags
	Window = SDL_CreateWindow("Glotón Galáctico", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (Window == NULL)
	{
		SDL_Log("Unable to create window: %s", SDL_GetError());
		return false;
	}
	//Create a 2D rendering context for a window: window, device index, flags
	Renderer = SDL_CreateRenderer(Window, -1, 0);
	if (Renderer == NULL)
	{
		SDL_Log("Unable to create rendering context: %s", SDL_GetError());
		return false;
	}
	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
	//Initialize keys array
	for (int i = 0; i < MAX_KEYS; ++i)
		keys[i] = KEY_IDLE;

	//Load images
	if (!LoadImages())
		return false;
	int flags = MIX_INIT_OGG;
	if (Mix_Init(flags) != flags) {
		SDL_Log("Failed to init OGG module for SDL_Mixer!\n");
		SDL_Log("Mix_Init: %s\n", Mix_GetError());
		return false;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		SDL_Log("Failed to init SDL_Mixer!\n");
		SDL_Log("Mix_OpenAudio: %s\n", Mix_GetError());
		return false;
	}

	if (!LoadAudios())
		return false;

	//Init variables
	//size: 104x82
	Player.Init(500, 1200>> 1, 150, 150, 7);
	Player.SetColliderSize(150, 100);
	
	Meteor.Init(700, -100, 125, 130,2);
	Meteor.SetColliderSize(115, 100);

	Candy.Init(32, 512, 80, 70, 3);
	Candy.SetColliderSize(80, 70);


	srand(time(NULL));

	idx_obj = 0;
	int w;
	SDL_QueryTexture(img_background, NULL, NULL, &w, NULL);
	Scene.Init(0, 0, w, WINDOW_HEIGHT, 4);
	god_mode = false;

	return true;
}
bool Game::LoadAudios() {
	num_tracks = 0;
	tracks[num_tracks++] = Mix_LoadMUS("Roboxel - Space Music (Remix).ogg");

	Mix_PlayMusic(tracks[0], -1);

	sfxs[num_sfxs++] = Mix_LoadWAV("sample_wav.wav");
	
	return true;
}
bool Game::LoadImages()
{
	if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		SDL_Log("IMG_Init, failed to init required png support: %s\n", IMG_GetError());
		return false;
	}
	img_menu = SDL_CreateTextureFromSurface(Renderer, IMG_Load("menu.png"));				//Load menu
	if (img_menu == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_background = SDL_CreateTextureFromSurface(Renderer, IMG_Load("background.png"));
	if (img_background == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_player = SDL_CreateTextureFromSurface(Renderer, IMG_Load("spaceship-r.png"));
	if (img_player == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_obj = SDL_CreateTextureFromSurface(Renderer, IMG_Load("asteroide.png"));
	if (img_obj == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_can = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Candy.png"));
	if (img_can == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_over = SDL_CreateTextureFromSurface(Renderer, IMG_Load("over.png"));
	if (img_over == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	return true;
}
void Game::Release()
{
	//Release images
	SDL_DestroyTexture(img_menu);
	SDL_DestroyTexture(img_background);
	SDL_DestroyTexture(img_player);
	SDL_DestroyTexture(img_obj);
	SDL_DestroyTexture(img_can);
	SDL_DestroyTexture(img_over);
	IMG_Quit();
	
	for (int i = 0; i < num_tracks; ++i)
		Mix_FreeMusic(tracks[i]);
	for (int i = 0; i < num_sfxs; ++i)
		Mix_FreeChunk(sfxs[i]);

	
	Mix_CloseAudio();
	while (Mix_Init(0))
		Mix_Quit();

	//Clean up all SDL initialized subsystems
	SDL_Quit();
}
bool Game::Input()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)	return false;
	}

	SDL_PumpEvents();
	const Uint8* keyboard = SDL_GetKeyboardState(NULL);
	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keyboard[i])
			keys[i] = (keys[i] == KEY_IDLE) ? KEY_DOWN : KEY_REPEAT;
		else
			keys[i] = (keys[i] == KEY_REPEAT || keys[i] == KEY_DOWN) ? KEY_UP : KEY_IDLE;
	}

	return true;
}
bool Game::Update()
{
	
	current_time = SDL_GetTicks();

	//Read Input
	if (!Input())	return true;

	//Process Input
	
	if (keys[SDL_SCANCODE_ESCAPE] == KEY_DOWN)	return true;
	if (keys[SDL_SCANCODE_RETURN] == KEY_DOWN)
	{
		Mix_PauseMusic();
		enter = true;
	}
	if (keys[SDL_SCANCODE_F1] == KEY_DOWN)		god_mode = !god_mode;

	if (Player.IsColliding(ColliderTest) == true)
		{
			if (keys[SDL_SCANCODE_RIGHT] == KEY_REPEAT && Player.IsColliding(ColliderTest) == false)
			{
				
			}
		}
	if (keys[SDL_SCANCODE_SPACE] == KEY_DOWN)
	{
		int x, y, w, h;
		x = 60;
		y = 60;
		w = 70;
		h = 70;
	}

	if (firstime_showing_game && enter == true)
	{
		firstime_showing_game = false;
		StartGame();
	}
	//Player update
	if (firstime_showing_game == false)
	{
		UpdateGame();

	}
	
		
	return false;
	
}
void Game::Draw()
{
	SDL_Rect rc;

	if (enter != true)
	{
		
		//Set the color used for drawing operations
		SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);						//Color blanco = (255, 255, 255, 255)
		//Clear rendering target
		SDL_RenderClear(Renderer);

		//Draw menu																	//Nuevo "Draw menu"
		Scene.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
		SDL_RenderCopy(Renderer, img_menu, NULL, &rc);

		SDL_RenderPresent(Renderer);

		//SDL_Delay(10);	// 1000/10 = 100 fps max
	}
	

	if (enter == true)
	{
		//God mode uses red wireframe rectangles for physical objects
		if (god_mode) SDL_SetRenderDrawColor(Renderer, 192, 0, 0, 255);

		//Draw scene
		Scene.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
		SDL_RenderCopy(Renderer, img_background, NULL, &rc);
		rc.x += rc.w;
		SDL_RenderCopy(Renderer, img_background, NULL, &rc);

		//Draw player
		Player.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
		SDL_RenderCopy(Renderer, img_player, NULL, &rc);
		if (god_mode) SDL_RenderDrawRect(Renderer, &rc);

		//Draw Meteo
		Meteor.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
		SDL_RenderCopy(Renderer, img_obj, NULL, &rc);
		
		//Draw Candy
		Candy.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
		SDL_RenderCopy(Renderer, img_can, NULL, &rc);
		
		//Draw shots
		
		DrawCollider();
		//Update screen
		SDL_RenderPresent(Renderer);

		SDL_Delay(10);	// 1000/10 = 100 fps max
	}
}void Game::DrawCollider()
{
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(Renderer, &r, &g, &b, &a);
	SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);

	if (Player.is_coliding)
	{
		SDL_SetRenderDrawColor(Renderer, 3, 25, 255, 255);
	}
	else
	{
		SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
	}

	SDL_Rect rect = Player.GetColliderRect();
	SDL_RenderDrawRect(Renderer, &rect);
	
	SDL_Rect collider_rect = Meteor.GetColliderRect();
	SDL_RenderDrawRect(Renderer, &collider_rect);
	
	//SDL_Rect collider_rect = Candy.GetColliderRect();
	//SDL_RenderDrawRect(Renderer, &collider_rect);


	SDL_SetRenderDrawColor(Renderer, r, g, b, a);
}

void Game::CheckCollider()
{
	SDL_Rect rc;

	bool is_coliding1 = Player.IsColliding(Candy);

	if (is_coliding1)
	{
		int r = rand() % WINDOW_WIDTH;
		start_meteor_time = current_time;
		Candy.SetPos(r, -100);
		restart_meteor = true;
	}
	
	bool is_coliding = Player.IsColliding(Meteor);

	if (is_coliding)
	{
		int r = rand() % WINDOW_WIDTH;
		start_meteor_time = current_time;
		Meteor.SetPos(r, -100);
		restart_meteor = true;
		
		vida-=1;
		
		if (vida <= 0)
		{
			//Set the color used for drawing operations
			SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);						//Color blanco = (255, 255, 255, 255)
			//Clear rendering target
			SDL_RenderClear(Renderer);

			//Draw menu																	//Nuevo "Draw menu"
			Scene.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
			SDL_RenderCopy(Renderer, img_over, NULL, &rc);

			SDL_RenderPresent(Renderer);

			SDL_Delay(1000);	

			Player.ShutDown();
		}
	}

}

void Game::UpdateGame()
{
	int fx = 0, fy = 0;
	if (keys[SDL_SCANCODE_LEFT] == KEY_REPEAT && Player.GetX() > 0)
	{
		//SDL_DestroyTexture(img_player);
		//img_player = SDL_CreateTextureFromSurface(Renderer, IMG_Load("spaceship-l.png"));
		fx = -1;
	}
	if (keys[SDL_SCANCODE_RIGHT] == KEY_REPEAT && Player.GetX() < 1020 - Player.GetWidth())
	{
		//SDL_DestroyTexture(img_player);
		//img_player = SDL_CreateTextureFromSurface(Renderer, IMG_Load("spaceship-r.png"));
		fx = 1;
	}
	Player.Move(fx, fy);

	//Mateorito
	if (current_time > start_meteor_time + 0050)
	{
		if( restart_meteor == true)
		{
			restart_meteor = false;
			
		}
		Meteor.Move(0, 4);
	}
	if (Meteor.GetY() >=850)
	{
		int r = rand() % WINDOW_WIDTH;
		start_meteor_time = current_time;
		Meteor.SetPos(r, -100);
		restart_meteor = true;
	}
	
	//Candy
	if (current_time > start_meteor_time + 1)
	{
		if (restart_meteor == true)
		{
		
			restart_meteor = false;

		}
		Candy.Move(0, 2);
	}
	if (Candy.GetY() >= 850)
	{
		int r = rand() % WINDOW_WIDTH;
		start_meteor_time = current_time;
		Candy.SetPos(r, -100);
		restart_meteor = true;
	}
	
	if (Player.IsAlive() == false)
	{
		finish = true;
	}
	
	CheckCollider();
}
void Game::StartGame()
{
	start_meteor_time = current_time;

}


