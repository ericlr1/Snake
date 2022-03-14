#include "Game.h"
#include <math.h>

bool enter = false;
int RandObj = 0;

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
	//Initialize keys array
	for (int i = 0; i < MAX_KEYS; ++i)
		keys[i] = KEY_IDLE;

	//Load images
	if (!LoadImages())
		return false;

	//Init variables
	//size: 104x82
	Player.Init(500, 1200>> 1, 104, 82, 5);
	//Definicion de tamaño
	
	Player.SetColliderSize(104, 82);
		
	idx_obj = 0;
	int w;
	SDL_QueryTexture(img_background, NULL, NULL, &w, NULL);
	Scene.Init(0, 0, w, WINDOW_HEIGHT, 4);
	god_mode = false;

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
	return true;
}
void Game::Release()
{
	//Release images
	SDL_DestroyTexture(img_menu);
	SDL_DestroyTexture(img_background);
	SDL_DestroyTexture(img_player);
	SDL_DestroyTexture(img_obj);
	IMG_Quit();
	
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
	
	//Read Input
	if (!Input())	return true;

	//Process Input
	int fx = 0, fy = 0;
	if (keys[SDL_SCANCODE_ESCAPE] == KEY_DOWN)	return true;
	if (keys[SDL_SCANCODE_RETURN] == KEY_DOWN)
	{
		enter = true;
	}
	if (keys[SDL_SCANCODE_F1] == KEY_DOWN)		god_mode = !god_mode;

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

	if (Player.IsColliding(ColliderTest) == true)
		{
			if (keys[SDL_SCANCODE_RIGHT] == KEY_REPEAT && Player.IsColliding(ColliderTest) == false)
			{
				
			}
		}
	if (RandObj == 0)
	{
		int x, y, w, h;
		//size: 56x20
		//offset from player: dx, dy = [(29, 3), (29, 59)]
		Object[idx_obj].Init(60, 60, 100, 100, 10);
		idx_obj %= MAX_SHOTS;
	}

	//Logic
	//Scene scroll
	/*if (enter == true)
	{
		Scene.Move(-1, 0);
		if (Scene.GetX() <= -Scene.GetWidth())	Scene.SetX(0);
	}*/

	
	//Player update
	Player.Move(fx, fy);
	//Shots update
	for (int i = 0; i < MAX_SHOTS; ++i)
	{
		if (Object[i].IsAlive())
		{
			Object[i].Move(0, 1);
			if (Object[i].GetX() > WINDOW_WIDTH)	Object[i].ShutDown();
		}
	}
		CheckCollider();
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

		//Draw shots
		for (int i = 0; i < MAX_SHOTS; ++i)
		{
			if (Object[i].IsAlive())
			{
				Object[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
				SDL_RenderCopy(Renderer, img_obj, NULL, &rc);
				if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
			}
		}
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
	
	SDL_Rect collider_rect = ColliderTest.GetColliderRect();
	SDL_RenderDrawRect(Renderer, &collider_rect);


	SDL_SetRenderDrawColor(Renderer, r, g, b, a);
}

void Game::CheckCollider()
{
	Player.is_coliding = Player.IsColliding(ColliderTest);
}
