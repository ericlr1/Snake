#include "Entity.h"


Entity::Entity()
{
	is_alive = false;
}
Entity::~Entity()
{
}
void Entity::Init(int posx, int posy, int w, int h, int s) 
{
	x = posx;
	y = posy;
	width = w;
	height = h;
	speed = s;
	is_alive = true;
}
void Entity::GetRect(int *posx, int *posy, int *w, int *h)
{
	*posx = x;
	*posy = y;
	*w = width;
	*h = height;
}
int Entity::GetX()
{
	return x;
}
int Entity::GetWidth()
{
	return width;
}
void Entity::SetX(int posx)
{
	x = posx;
}
void Entity::ShutDown()
{
	is_alive = false;
}
bool Entity::IsAlive()
{
	return is_alive;
}

void Entity::Move(int dx, int dy)
{
	x += dx * speed;
	y += dy * speed;
}
void Entity::SetColliderSize(int x, int y)
{
	collider.height = y;
	collider.width = x;
}

SDL_Rect Entity::GetColliderRect()
{
	SDL_Rect rect;

	rect.x = x;
	rect.y = y;
	rect.w = collider.width;
	rect.h = collider.height;

	return rect;
}

bool Entity::IsColliding(Entity entity)
{
	SDL_Rect rect1 = GetColliderRect();
	SDL_Rect rect2 = entity.GetColliderRect();

	if (rect1.x <= rect2.x && rect1.w + rect1.x >= rect2.x)
	{
		if (rect1.y <= rect2.y && rect1.h + rect1.y >= rect2.y)
		{
			return true;

		}
	}

	if (rect1.x <= rect2.x + rect2.w && rect1.w + rect1.x >= rect2.x + rect2.w)
	{
		if (rect1.y <= rect2.y && rect1.h + rect1.y >= rect2.y)
		{
			return true;

		}
	}

	if (rect1.w + rect1.x >= rect2.w + rect2.x && rect1.x <= rect2.w + rect2.x)
	{
		if (rect1.h + rect1.y >= rect2.h + rect2.y && rect1.y <= rect2.h + rect2.y) 
		{
			return true;
		}
	}

	if (rect1.x <= rect2.x && rect1.w + rect1.x >= rect2.x)
	{
		if (rect1.h + rect1.y >= rect2.h + rect2.y && rect1.y <= rect2.h + rect2.y)
		{
			return true;
		}
	}
	
	return false;
}
