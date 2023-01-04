#include "Goomba.h"

Goomba::Goomba() :Enemy()
{
	isEnable = true;
}

Goomba::Goomba(float x, float y):Enemy(x, y)
{
	isEnable = true;
	SetState(GOOMBA_STATE_WALKING);
}

void Goomba::GetBoundingBox(float &left, float &top, float &right, float &bottom, bool isEnable)
{
	if (isEnable == true)
	{
		left = x;
		top = y;
		right = x + GOOMBA_BBOX_WIDTH;

		if (state == GOOMBA_STATE_DIE)
			bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
		else
			bottom = y + GOOMBA_BBOX_HEIGHT;
	}
	else
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}
	
}

void Goomba::OnNoCollision(DWORD dt)
{
	x += dx;
	y += dy;
};

void Goomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return; 
	if (dynamic_cast<Goomba*>(e->obj)) return; 

	if (e->ny != 0 && state != GOOMBA_STATE_DIE_NX)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void Goomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (state == GOOMBA_STATE_INACTIVE)
		return;
	
	if (this->IsDead() == true && (GetTickCount64() - time_death > GOOMBA_INACTIVE_TIME) )
	{
		this->SetState(GOOMBA_STATE_INACTIVE);
		isDeleted = true;
		return;
	}
	Enemy::Update(dt, coObjects);
	if (this->state != GOOMBA_STATE_DIE_NY)
		vy += dt * GOOMBA_GRAVITY;

	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void Goomba::Render()
{
	if (state != GOOMBA_STATE_INACTIVE)
	{
		int ani;
		ani = GOOMBA_ANI_WALKING;
		if (state == GOOMBA_STATE_DIE_NY) {
			ani = GOOMBA_ANI_DIE;
		}
		animation_set->at(ani)->Render(nx, x, y);
	}

	//RenderBoundingBox();
}

void Goomba::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE:
			y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE)/2;
			vx = 0;
			vy = 0;
			break;
		case GOOMBA_STATE_WALKING: 
			vx = -GOOMBA_WALKING_SPEED;
			break;
		case GOOMBA_STATE_DIE_NX:
			vy = -GOOMBA_DIE_DEFLECT_SPEED;
			vx = 0;
			isEnable = false;
			break;
		case GOOMBA_STATE_DIE_NY:
			y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE) / 2;
			vx = 0;
			vy = 0;
			isEnable = false;
			break;
		case GOOMBA_STATE_INACTIVE:
			vx = 0;
			isEnable = false;
			break;
	}
}

bool Goomba::IsDead()
{
	if (this->state == GOOMBA_STATE_DIE_NY || this->state == GOOMBA_STATE_DIE_NX)
	{
		return true;
	}
	return false;
}

void Goomba::SetDie(bool n)
{
	if (n == true)
	{
		this->SetState(GOOMBA_STATE_DIE_NX);
	}
	else
		this->SetState(GOOMBA_STATE_DIE_NY);
	time_death = GetTickCount64();
}