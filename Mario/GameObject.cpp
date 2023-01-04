#include <d3dx9.h>
#include <algorithm>

#include "Utils.h"
#include "debug.h"
#include "Textures.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprites.h"

GameObject::GameObject()
{
	x = y = 0;
	vx = vy = 0;
	nx = 1;
	ny = -1;
	isEnable = true;
	isDeleted = false;
}

void GameObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	this->dt = dt;
	dx = vx * dt;
	dy = vy * dt;
}

void GameObject::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = Textures::GetInstance()->Get(ID_TEX_BBOX);

	float l,t,r,b; 

	GetBoundingBox(l, t, r, b, this->isEnable);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	Game::GetInstance()->Draw(x, y, bbox, rect.left, rect.top, rect.right, rect.bottom, 0);
}

GameObject::~GameObject()
{

}