#include "Enemy.h"

Enemy::Enemy()
{

}

Enemy::Enemy(float x, float y) :GameObject(x, y)
{

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
	this->entrY = x;
	this->entrY = y;
}

