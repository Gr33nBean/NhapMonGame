#pragma once

#include "GameObject.h"
#include "Animations.h"
#include "ThongSo.h"

#define ID_ANI_COIN 11000

#define	COIN_WIDTH 10
//#define COIN_BBOX_WIDTH 10
//#define COIN_BBOX_HEIGHT 16

class Coin : public GameObject {
	virtual void GetBoundingBox(float& left, float& top,
		float& right, float& bottom, bool isEnable);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	//virtual void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects,
	//	vector<LPCOLLISIONEVENT>& coEvents);
public:
	virtual void SetState(int state);
	Coin() { this->SetState(COIN_STATE_ACTIVE); };
	void Disappearance();
};