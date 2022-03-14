#pragma once
#include "ColliderType.h"
#include "Collider.h"
#include "SDL/include/SDL_rect.h"

class Entity
{
public:
	Entity();
	~Entity();

	void Init(int posx, int posy, int w, int h, int s);
	void GetRect(int *posx, int *posy, int *w, int *h);
	void SetX(int posx);
	int  GetX();
	int  GetWidth();
	void ShutDown(); 
	bool IsAlive();
	void Move(int dx, int dy);
	
//Inicio de funcion
	void SetColliderSize(int x, int y);

	SDL_Rect GetColliderRect();

	bool IsColliding(Entity collider);

public:
	bool is_coliding = false;

private:
	int x, y;
	int width, height;
	int speed;
	bool is_alive;
	Collider collider
};

