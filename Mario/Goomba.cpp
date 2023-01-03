#include "Goomba.h"

CGoomba::CGoomba(float x, float y):Enemy(x, y)
{
	this->ax = 0;
	this->ay = GOOMBA_GRAVITY;
	isEnable = true;
	SetState(GOOMBA_STATE_WALKING);
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom, bool isEnable)
{
	if (isEnable == true)
	{
		if (state == GOOMBA_STATE_DIE)
		{
			left = x - GOOMBA_BBOX_WIDTH / 2;
			top = y - GOOMBA_BBOX_HEIGHT_DIE / 2;
			right = left + GOOMBA_BBOX_WIDTH;
			bottom = top + GOOMBA_BBOX_HEIGHT_DIE;
		}
		else
		{
			left = x - GOOMBA_BBOX_WIDTH / 2;
			top = y - GOOMBA_BBOX_HEIGHT / 2;
			right = left + GOOMBA_BBOX_WIDTH;
			bottom = top + GOOMBA_BBOX_HEIGHT;
		}
	}
	else
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}
	
}

void CGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return; 
	if (dynamic_cast<CGoomba*>(e->obj)) return; 

	if (e->ny != 0 && state != GOOMBA_STATE_DIE_NX)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (state == GOOMBA_STATE_INACTIVE)
		return;
	
	if (this->IsDead() == true && (GetTickCount64() - time_death > GOOMBA_INACTIVE_TIME) )
	{
		this->SetState(GOOMBA_STATE_INACTIVE);
		isDeleted = true;
		return;
	}

	vy += ay * dt;
	vx += ax * dt;
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CGoomba::Render()
{
	if (state != GOOMBA_STATE_INACTIVE)
	{
		int aniId;
		aniId = ID_ANI_GOOMBA_WALKING;
		if (state == GOOMBA_STATE_DIE_NY) {
			aniId = ID_ANI_GOOMBA_DIE;
		}
		CAnimations::GetInstance()->Get(aniId)->Render(x, y);

	}

	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE:
			y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE)/2;
			vx = 0;
			vy = 0;
			ay = 0; 
			break;
		case GOOMBA_STATE_WALKING: 
			vx = -GOOMBA_WALKING_SPEED;
			break;
		case GOOMBA_STATE_DIE_NX:
			vy = -GOOMBA_DIE_DEFLECT_SPEED;
			vx = 0;
			ax = 0;
			isEnable = false;
			break;
		case GOOMBA_STATE_DIE_NY:
			y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE) / 2;
			vx = 0;
			vy = 0;
			ay = 0;
			isEnable = false;
			break;
		case GOOMBA_STATE_INACTIVE:
			vx = 0;
			ax = 0;
			isEnable = false;
			break;
	}
}

bool CGoomba::IsDead()
{
	if (this->state == GOOMBA_STATE_DIE_NY || this->state == GOOMBA_STATE_DIE_NX)
	{
		return true;
	}
	return false;
}

void CGoomba::SetDie(bool n)
{
	if (n == true)
	{
		this->SetState(GOOMBA_STATE_DIE_NX);
	}
	else
		this->SetState(GOOMBA_STATE_DIE_NY);
	time_death = GetTickCount64();
}