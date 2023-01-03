#include "Enemy.h"

Enemy::Enemy()
{

}

Enemy::Enemy(float x, float y) :CGameObject(x, y)
{

}

Enemy::~Enemy()
{

}

void Enemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
}

void Enemy::SetState(int state)
{
	CGameObject::SetState(state);
}

void Enemy::SetEntryPosition(int x, int y)
{
	this->entrY = x;
	this->entrY = y;
}

