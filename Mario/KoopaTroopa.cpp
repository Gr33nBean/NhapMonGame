#include "KoopaTroopa.h"
#include "debug.h"
#include "Goomba.h"
#include "Brick.h"

CKoopaTroopa::CKoopaTroopa(float x, float y) :Enemy(x, y)
{
	this->ax = 0;
	this->ay = KOOPATROOPA_GRAVITY;
	isPickedUp = false;
	SetState(KOOPATROOPA_STATE_WALKING);
}

void CKoopaTroopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

	if (state == KOOPATROOPA_STATE_HIDING)
	{
		left = x - KOOPATROOPA_BBOX_WIDTH / 2;
		top = y - KOOPATROOPA_BBOX_HEIGHT_HIDING / 2;
		right = left + KOOPATROOPA_BBOX_WIDTH;
		bottom = top + KOOPATROOPA_BBOX_HEIGHT_HIDING;
	}
	else
	{
		left = x - KOOPATROOPA_BBOX_WIDTH / 2;
		top = y - KOOPATROOPA_BBOX_HEIGHT / 2;
		right = left + KOOPATROOPA_BBOX_WIDTH;
		bottom = top + KOOPATROOPA_BBOX_HEIGHT;
	}
}

void CKoopaTroopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;
	Enemy::Update(dt, coObjects);

	//Nếu đang rinh cái mai
	if (isPickedUp == true)
	{
		if (mario->isPickingUp == true)
		{
			this->vx = mario->vx;
			this->vy = mario->vy;
			this->nx = mario->nx;
			this->ny = mario->ny;
		}
		else // thả cái mai ra
		{
			isPickedUp = false;
			this->SetState(KOOPATROOPA_STATE_HIDING);
		}

	}
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CKoopaTroopa::Render()
{
	int ani = KOOPATROOPA_ANI_WALKING;
	if (state == KOOPATROOPA_STATE_HIDING)
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
			nx = -1;
			break;
		case KOOPATROOPA_STATE_HIDING:
			y += (KOOPATROOPA_BBOX_HEIGHT - KOOPATROOPA_BBOX_HEIGHT_HIDING)/2;
			vx = 0;
			vy = 0;
			ay = 0;
			break;
		case KOOPATROOPA_STATE_DIE_NX:
			vy = -KOOPATROOPA_DIE_DEFLECT_SPEED;
			vx = 0;
			ax = 0;
			break;
	}
}

void CKoopaTroopa::SetDie(bool n)
{
	// true: nx. false = ny
	if (n == true)
		this->SetState(KOOPATROOPA_STATE_DIE_NX);
	else
		this->SetState(KOOPATROOPA_STATE_HIDING);
}

bool CKoopaTroopa::IsDead()
{
	if (this->state == KOOPATROOPA_STATE_HIDING)
	{
		return true;
	}
	return false;
}

void CKoopaTroopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CKoopaTroopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	//if (!e->obj->IsBlocking()) return;
	//if (dynamic_cast<CKoopaTroopa*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (this->IsHiding() && dynamic_cast<Enemy*>(e->obj))
	{
		Enemy* enemy = dynamic_cast<Enemy*>(e->obj);
		if (e->nx != 0)
		{
			enemy->SetDie(true);
			this->SetState(KOOPATROOPA_STATE_DIE_NX);
		}
	}
	else if (e->nx != 0)
	{
		// Code cái mà con rùa xanh thông minh!
		vx = -vx;
	}
}

void CKoopaTroopa::IsKicked(int nx)
{
	this->nx = nx;
	if (this->nx < 0)
	{
		this->vx = -KOOPATROOPA_BUMP_SPEED;
	}
	else
		this->vx = KOOPATROOPA_BUMP_SPEED;
}

bool CKoopaTroopa::IsHiding()
{
	if (this->state == KOOPATROOPA_STATE_HIDING)
		return true;
	return false;
}