#pragma once
#include"GameObject.h"
#include "ThongSo.h"

class FireBall : public GameObject
{
	DWORD explodeTime;
public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b, bool isEnable);
	virtual void SetState(int state);
	FireBall(int _x, int _y, int _nx)
	{
		this->x = _x;
		this->y = _y;
		this->isEnable = true;
		AnimationSets* animation_sets = AnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(60);
		this->SetAnimationSet(ani_set);
		this->nx = _nx;
		this->SetState(FIREBALL_STATE_NORMAL);
	}
};
typedef FireBall* LPFireBall;