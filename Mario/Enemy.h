#pragma once
#include"GameObject.h"
class Enemy :public CGameObject
{
protected:
	DWORD time_death;
	int entryX, entrY;
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render() = 0;
	virtual void SetState(int state);
	virtual bool IsDead() = 0;

	virtual	void SetDie(bool n) = 0; // nx == true; ny == false
	virtual void SetEntryPosition(int x, int y);
	virtual void SetReborn() {};
	virtual void SetEnable() {};
	virtual void IsEnable() {};

	Enemy();
	Enemy(float x, float y);
	~Enemy();
};

