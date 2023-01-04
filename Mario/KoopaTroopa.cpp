#include "KoopaTroopa.h"
#include "debug.h"
#include "Goomba.h"
#include "Brick.h"
#include "Utils.h"

KoopaTroopa::KoopaTroopa() :Enemy()
{
	isPickedUp = false;
	isEnable = true;
}

KoopaTroopa::KoopaTroopa(float x, float y) :Enemy(x, y)
{
	isPickedUp = false;
	isEnable = true;
	SetState(KOOPATROOPA_STATE_WALKING);
}

void KoopaTroopa::GetBoundingBox(float& left, float& top, float& right, float& bottom, bool isEnable)
{
	if (isEnable == true)
	{
		left = x;
		top = y;
		right = x + KOOPATROOPA_BBOX_WIDTH;

		if (state == KOOPATROOPA_STATE_HIDING)
			bottom = y + KOOPATROOPA_BBOX_HEIGHT_HIDING;
		else
			bottom = y + KOOPATROOPA_BBOX_HEIGHT;
	}
	else
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}
	
}

void KoopaTroopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Enemy::Update(dt, coObjects);
	vy += KOOPATROOPA_GRAVITY * dt;

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

void KoopaTroopa::Render()
{
	int ani;
	if (state == KOOPATROOPA_STATE_WALKING)
		ani = KOOPATROOPA_ANI_WALKING;
	else
		ani = KOOPATROOPA_ANI_HIDING;
	animation_set->at(ani)->Render(nx, x, y);
}

void KoopaTroopa::SetState(int state)
{
	GameObject::SetState(state);
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
			break;
		case KOOPATROOPA_STATE_DIE_NX:
			vy = -KOOPATROOPA_DIE_DEFLECT_SPEED;
			isEnable = false;
			vx = 0;
			break;
	}
}

void KoopaTroopa::SetDie(bool n)
{
	// true: nx. false = ny
	if (n == true)
		this->SetState(KOOPATROOPA_STATE_DIE_NX);
	else
		this->SetState(KOOPATROOPA_STATE_HIDING);
}

bool KoopaTroopa::IsDead()
{
	if (this->state == KOOPATROOPA_STATE_HIDING)
	{
		return true;
	}
	return false;
}

void KoopaTroopa::OnNoCollision(DWORD dt)
{
	x += dx;
	y += dy;
};

void KoopaTroopa::OnCollisionWith(LPCOLLISIONEVENT e)
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
			if (isPickedUp == true)
			{
				this->SetState(KOOPATROOPA_STATE_DIE_NX);
			}
			enemy->SetDie(true);
		}
	}
	else if (e->nx != 0)
	{
		// Code cái mà con rùa xanh thông minh!
		vx = -vx;
	}
}

void KoopaTroopa::IsKicked(int nx)
{
	this->nx = nx;
	if (this->nx < 0)
	{
		this->vx = -KOOPATROOPA_BUMP_SPEED;
	}
	else
		this->vx = KOOPATROOPA_BUMP_SPEED;
}

bool KoopaTroopa::IsHiding()
{
	if (this->state == KOOPATROOPA_STATE_HIDING)
		return true;
	return false;
}