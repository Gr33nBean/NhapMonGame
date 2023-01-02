#include "KoopaTroopa.h"

CKoopaTroopa::CKoopaTroopa(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = 0.002;
	SetState(KOOPATROOPA_STATE_WALKING);
}

void CKoopaTroopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + KOOPATROOPA_BBOX_WIDTH;

	if (state == KOOPATROOPA_STATE_HIDING)
		bottom = y + KOOPATROOPA_BBOX_HEIGHT_HIDING;
	else
		bottom = y + KOOPATROOPA_BBOX_HEIGHT;
}

void CKoopaTroopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y += ay * dt;
	x += ax * dt;
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CKoopaTroopa::Render()
{
	int ani;
	if (state == KOOPATROOPA_STATE_WALKING)
		ani = KOOPATROOPA_ANI_WALKING;
	else
		ani = KOOPATROOPA_ANI_HIDING;
	CAnimations::GetInstance()->Get(ani)->Render(x, y);
}

void CKoopaTroopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPATROOPA_STATE_WALKING:
		vx = -KOOPATROOPA_WALKING_SPEED;
		break;
	case KOOPATROOPA_ANI_HIDING:
		vx = 0;
		vy = 0;
		break;
	}
}

void CKoopaTroopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CKoopaTroopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CKoopaTroopa*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}