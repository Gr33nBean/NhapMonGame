#pragma once
#include "GameObject.h"
#include "Enemy.h"
#include "ThongSo.h"

class Goomba : public Enemy
{
public:

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom, bool isEnable);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

	virtual void SetState(int state);
	virtual bool IsDead();
	virtual void SetBeingStromped();
	virtual void SetBeingSkilled();
	virtual void EnableAgain();
	Goomba(float x, float y);
	Goomba();
};