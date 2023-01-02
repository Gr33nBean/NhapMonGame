#pragma once
#include"GameObject.h"


#define KOOPATROOPA_WALKING_SPEED 0.05f
#define KOOPATROOPA_BBOX_WIDTH 16
#define KOOPATROOPA_BBOX_HEIGHT 26
#define KOOPATROOPA_BBOX_HEIGHT_HIDING 16

#define KOOPATROOPA_STATE_WALKING 100
#define KOOPATROOPA_STATE_HIDING 200

#define KOOPATROOPA_ANI_WALKING 0
#define KOOPATROOPA_ANI_HIDING 1
class CKoopaTroopa :public CGameObject
{
protected:
	float ax;
	float ay;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
public:
	CKoopaTroopa(float x, float y);
	virtual void SetState(int state);
};
