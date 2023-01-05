#pragma once
#include "GameObject.h"
#include "Enemy.h"

#define GOOMBA_WALKING_SPEED 0.05f
#define GOOMBA_GRAVITY 0.002f
#define GOOMBA_DIE_DEFLECT_SPEED 0.5f

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 16
#define GOOMBA_BBOX_HEIGHT_DIE 9

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200
#define GOOMBA_STATE_DIE_NX 300
#define GOOMBA_STATE_DIE_NY 400
#define GOOMBA_STATE_INACTIVE 500

#define GOOMBA_ANI_WALKING 0
#define GOOMBA_ANI_DIE 1

#define GOOMBA_INACTIVE_TIME 500
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
	virtual void SetDie(bool n);
	Goomba(float x, float y);
	Goomba();
};