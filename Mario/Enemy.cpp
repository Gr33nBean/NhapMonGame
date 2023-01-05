#include "Enemy.h"

Enemy::Enemy():GameObject()
{

}

Enemy::Enemy(float x, float y) :GameObject(x, y)
{
	this->x = x; 
	this->y = y;
	this->SetEntryPosition(x, y);
}

Enemy::~Enemy()
{

}

void Enemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt, coObjects);
}

void Enemy::SetState(int state)
{
	GameObject::SetState(state);
}

void Enemy::SetEntryPosition(int x, int y)
{
	this->entryX = x;
	this->entryY = y;
}

bool Enemy::IsAbleToActive()
{
	if (isEnable = false)
	{
		Game* game = Game::GetInstance();
		int widthX = game->GetCamX() + game->GetScreenWidth();
		int widthY = game->GetCamY() + game->GetScreenHeight();
		if (entryX > widthX || entryX < game->GetCamX())
			return true;
	}
	return false;
}
