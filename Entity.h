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
	int  GetY();
	int  GetX();
	int  GetWidth();
	void ShutDown(); 
	bool IsAlive();
	void Move(int dx, int dy);
	void SetPos(int posx, int posy);

//Inicio de funcion
	void SetColliderSize(int w, int h);
	void SetColliderPos(int posx, int posy);
	SDL_Rect GetColliderRect();

	bool IsColliding(Entity collider);

public:
	bool is_coliding = false;

private:
	int x, y;
	int width, height;
	int speed;
	bool is_alive;
	Collider collider;
};

