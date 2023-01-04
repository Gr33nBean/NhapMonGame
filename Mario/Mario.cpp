#include <algorithm>
#include "debug.h"
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include "Enemy.h"
#include "Goomba.h"
#include "KoopaTroopa.h"
#include "Coin.h"
#include "Portal.h"
#include"Block.h"
#include "Ground.h"
#include "Brick.h"
#include "Pipe.h"

#include "Collision.h"

void Mario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	GameObject::Update(dt);
	vy += MARIO_GRAVITY * dt;

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	isInGround = false;

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void Mario::Render()
{
	int ani = this->form;
	if (state == MARIO_STATE_DEATH)
		ani = MARIO_ANI_DIE;
	if (vx != 0 && isInGround == true)
	{
		ani += 4;
	}
	if (!isInGround)
		ani += 8;
	int alpha = 255;
	if (untouchable) alpha = 128;
	animation_set->at(ani)->Render(nx, x, y, alpha);
	RenderBoundingBox();

	//RenderBoundingBox();

	DebugOutTitle(L"Coins: %d", coin);
}

void Mario::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_WALKING:
		if (isSitting) break;
		vx = MARIO_WALKING_SPEED * nx;
		break;

	case MARIO_STATE_JUMPING:
		if (isSitting) break;
		isInGround = false;
		vy = -MARIO_JUMP_SPEED_Y;
		break;

	case MARIO_STATE_IDLE:
		vx = 0.0f;
		break;

	case MARIO_STATE_DEATH:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		vx = 0;
		break;

	case MARIO_STATE_RUNNING:
		if (isSitting) break;
		vx = (MARIO_WALKING_SPEED + (BUFF_SPEED * power_melter_stack)) * nx;
		break;
	
	case MARIO_STATE_BRAKING:
		vx = MARIO_BRAKE_DEFLECT_SPEED*nx;
		break;

	/*case MARIO_STATE_LONG_JUMPING:
		vy = -MARIO_LONG_JUMP_SPEED_Y;
		isInGround = false;
		break;*/

	case MARIO_STATE_KICK:
		break;

	/*case MARIO_STATE_SIT:
		if (isInGround && form != MARIO_SMALL_FORM)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y += MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;*/
	}
}

void Mario::GetBoundingBox(float& left, float& top, float& right, float& bottom, bool isEnable)
{
	left = x;
	top = y;

	if (form == MARIO_BIG_FORM)
	{
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
	}
	else if (form == MARIO_SMALL_FORM)
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
	else if (form == MARIO_FIRE_FORM)
	{
		right = x + MARIO_FIRE_BBOX_WIDTH;
		bottom = y + MARIO_FIRE_BBOX_HEIGHT;
	}
	else if (form == MARIO_RACCOON_FORM)
	{
		right = x + MARIO_RACCOON_BBOX_WIDTH;
		bottom = y + MARIO_RACCOON_BBOX_HEIGHT;
	}

}

void Mario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->form == MARIO_SMALL_FORM)
	{
		switch (l)
		{
		case MARIO_BIG_FORM:
			y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
			break;
		case MARIO_FIRE_FORM:
			y -= (MARIO_FIRE_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
			break;
		case MARIO_RACCOON_FORM:
			y -= (MARIO_RACCOON_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
			break;
		}
		
	}
	form = l;
}

void Mario::UpForm()
{
	int diffy = 0;
	if (form == MARIO_SMALL_FORM)
	{
		diffy = 16;

	}
	form += 1;
	if (form > MARIO_RACCOON_FORM)
	{
		diffy = -16;
		form = 0;
	}
	y -= diffy;
}

//void Mario::Jump()
//{
//	DWORD current = GetTickCount64();
//	if (current - long_jump_start > MARIO_LONG_JUMP_TIME && isInGround == true
//		&& long_jump_start != 0)
//	{
//		// nếu Mario chưa tiếp đất hoặc vật thì ko cho phép nhảy
//		this->SetState(MARIO_STATE_LONG_JUMPING);
//		isJump == false;
//		long_jump_start = 0;
//	}
//
//
//}
//void Mario::unJump()
//{
//	DWORD current = GetTickCount64();
//	if (current - long_jump_start < MARIO_LONG_JUMP_TIME && this->isInGround == true && isJump == true)
//	{
//		// nếu Mario chưa tiếp đất hoặc vật thì ko cho phép nhảy
//		// không cho phép nhảy ngắn khi Mario đã nhảy
//		this->SetState(MARIO_STATE_JUMPING);
//		DebugOut(L"\nxyz");
//
//	}
//	else
//	{
//		DebugOut(L"\nisInGround %d", isInGround);
//		DebugOut(L"\nisJump %d", isJump);
//		DebugOut(L"\nC: %d - L: %d", current, long_jump_start);
//	}
//	isJump = false;
//	long_jump_start = 0;
//}

void Mario::FillUpPowerMelter()
{
	DWORD current = GetTickCount64();
	if (stack_time_start == 0)
	{
		stack_time_start = current;
	}
	else
	{
		if (current - stack_time_start > STACK_TIME && power_melter_stack < POWER_METER_FULL)
		{
			power_melter_stack += 1;
			stack_time_start = 0;
			DebugOut(L"Power Meter : %d", power_melter_stack);
		}
	}
}
void Mario::LosePowerMelter()
{
	if (vx == 0)
	{
		DWORD current = GetTickCount64();
		if (stack_time_start == 0)
		{
			stack_time_start = current;
		}
		else
		{
			if (current - stack_time_start > STACK_TIME && power_melter_stack > 0)
			{
				power_melter_stack -= 1;
				stack_time_start = 0;
			}
		}
	}
}

void Mario::Information()
{
	DebugOut(L"\nX: %d, Y: %d", this->x, this->y);
	DebugOut(L"\nVx: %f, Vy: %f", this->vx, this->vy);
}
void Mario::PickUp()
{
	isPickingUp = true;

}

void Mario::SetDirect(bool nx)
{
	if (nx == true)
		this->nx = 1;
	else
		this->nx = -1;
}

// Collision
void Mario::OnNoCollision(DWORD dt)
{
	x += dx;
	y += dy;
}

void Mario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 )
	{
		vy = 0;
		if (e->ny < 0) isInGround = true;
	}
	else 
	if (e->nx != 0 && isPickingUp == false)
	{
		vx = 0;
	}
	if (dynamic_cast<Goomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<KoopaTroopa*>(e->obj))
		OnCollisionWithKoopaTroopa(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
}

void Mario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	Goomba* goomba = dynamic_cast<Goomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->IsDead() != true)
		{
			goomba->SetDie(false);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->IsDead() != true && goomba->IsEnable() == true)
			{
				if (form > MARIO_SMALL_FORM)
				{
					form -= 1;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DEATH);
				}
			}
		}
	}
}

void Mario::OnCollisionWithKoopaTroopa(LPCOLLISIONEVENT e)
{
	KoopaTroopa* troopa = dynamic_cast<KoopaTroopa*>(e->obj);

	// jump on top >> kill Troopa and deflect a bit 
	if (e->ny < 0)
	{
		if (troopa->IsDead() != true)
		{
			troopa->SetDie(false);
			isInGround = true;
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else  // hit by Troopa
	{
		if (untouchable == 0)
		{
			if (troopa->IsDead() != true && troopa->IsEnable() == true)
			{
				if (form > MARIO_SMALL_FORM)
				{
					form -= 1;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DEATH);
				}
			}
			else // Rinh và kick cái mai
			{
				if (isPickingUp == true)
				{
					troopa->PickUpBy(this);
				}
				else
				{
					troopa->isPickedUp = false;
					troopa->IsKicked(this->nx);
					this->SetState(MARIO_STATE_KICK);
				}
			}
		}
	}
}

void Mario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	coin++;
}

void Mario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	Game::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

////
//// Get animation ID for small Mario
////
//int Mario::GetAniIdSmall()
//{
//	int aniId = -1;
//	/*if (!isInGround)
//	{
//		if (abs(ax) == MARIO_ACCEL_RUN_X)
//		{
//			if (nx >= 0)
//				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
//			else
//				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
//		}
//		else
//		{
//			if (nx >= 0)
//				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
//			else
//				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
//		}
//	}
//	else*/
//		if (isSitting)
//		{
//			if (nx > 0)
//				aniId = ID_ANI_MARIO_BIG_SIT_RIGHT;
//			else
//				aniId = ID_ANI_MARIO_BIG_SIT_LEFT;
//		}
//		else
//			if (vx == 0)
//			{
//				if (nx > 0) aniId = MARIO_ANI_SMALL_IDLE;
//				else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
//				if (vy < 0)
//				{
//					if (nx < 0)
//					{
//						aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
//					}
//					else
//						aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
//				}
//			}
//			else if (vx > 0)
//			{
//				if (nx > 0)
//				{
//					if (power_melter_stack == POWER_METER_FULL)
//					{
//						aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
//					}
//					else
//						aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
//				}
//				else if (nx < 0)
//				{
//					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
//				}
//				if (vy < 0)
//				{
//					aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
//				}
//				if (this->state == MARIO_STATE_KICK)
//					aniId = ID_ANI_MARIO_SMALL_KICK_RIGHT;
//			}
//			else if (vx < 0)
//			{
//				
//				if (nx < 0)
//				{
//					if (power_melter_stack == POWER_METER_FULL)
//					{
//						aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
//					}
//					else
//						aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
//				}
//				else
//				{
//					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
//				}
//				if (vy < 0)
//				{
//					aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
//				}
//				if (this->state == MARIO_STATE_KICK)
//					aniId = ID_ANI_MARIO_SMALL_KICK_LEFT;
//			}
//
//	if (aniId == -1) aniId = MARIO_ANI_SMALL_IDLE;
//
//	return aniId;
//}
//
//
////
//// Get animdation ID for big Mario
////
//int Mario::GetAniIdBig()
//{
//	int aniId = -1;
//	/*if (!isInGround)
//	{
//		if (abs(ax) == MARIO_ACCEL_RUN_X)
//		{
//			if (nx >= 0)
//				aniId = ID_ANI_MARIO_BIG_JUMP_RUN_RIGHT;
//			else
//				aniId = ID_ANI_MARIO_BIG_JUMP_RUN_LEFT;
//		}
//		else
//		{
//			if (nx >= 0)
//				aniId = ID_ANI_MARIO_BIG_JUMP_WALK_RIGHT;
//			else
//				aniId = ID_ANI_MARIO_BIG_JUMP_WALK_LEFT;
//		}
//	}
//	else*/
//		if (isSitting)
//		{
//			if (nx > 0)
//				aniId = ID_ANI_MARIO_BIG_SIT_RIGHT;
//			else
//				aniId = ID_ANI_MARIO_BIG_SIT_LEFT;
//		}
//		else
//			if (vx == 0)
//			{
//				if (nx > 0) aniId = MARIO_ANI_BIG_IDLE;
//				else aniId = ID_ANI_MARIO_BIG_IDLE_LEFT;
//				if (vy < 0)
//				{
//					if (nx < 0)
//					{
//						aniId = ID_ANI_MARIO_BIG_JUMP_WALK_LEFT;
//					}
//					else
//						aniId = ID_ANI_MARIO_BIG_JUMP_WALK_RIGHT;
//				}
//			}
//			else if (vx > 0)
//			{
//
//				if (nx > 0)
//				{
//					if (power_melter_stack == POWER_METER_FULL)
//					{
//						aniId = ID_ANI_MARIO_BIG_RUNNING_RIGHT;
//					}
//					else
//						aniId = ID_ANI_MARIO_BIG_WALKING_RIGHT;
//				}
//				else if (nx < 0)
//				{
//					aniId = ID_ANI_MARIO_BIG_BRACE_RIGHT;
//				}
//
//				if (vy < 0)
//				{
//					aniId = ID_ANI_MARIO_BIG_JUMP_WALK_RIGHT;
//				}
//				if (this->state == MARIO_STATE_KICK)
//					aniId = ID_ANI_MARIO_BIG_KICK_RIGHT;
//			}
//			else if (vx < 0)
//			{
//				if (nx < 0)
//				{
//					if (power_melter_stack == POWER_METER_FULL)
//					{
//						aniId = ID_ANI_MARIO_BIG_RUNNING_LEFT;
//					}
//					else
//						aniId = ID_ANI_MARIO_BIG_WALKING_LEFT;
//				}
//				else
//				{
//					aniId = ID_ANI_MARIO_BIG_BRACE_LEFT;
//				}
//
//				if (vy < 0)
//				{
//					aniId = ID_ANI_MARIO_BIG_JUMP_WALK_LEFT;
//				}
//				if (this->state == MARIO_STATE_KICK)
//					aniId = ID_ANI_MARIO_BIG_KICK_LEFT;
//
//				/*if (ax > 0)
//					aniId = ID_ANI_MARIO_BIG_BRACE_LEFT;
//				else if ( power_melter_stack == POWER_METER_FULL)
//					aniId = ID_ANI_MARIO_BIG_RUNNING_LEFT;
//				else if (ax == -MARIO_ACCEL_WALK_X)
//					aniId = ID_ANI_MARIO_BIG_WALKING_LEFT;*/
//			}
//
//	if (aniId == -1) aniId = MARIO_ANI_BIG_IDLE;
//
//	return aniId;
//}
//
//
////
//// Get animdation ID for fire Mario
////
//int Mario::GetAniIdFire()
//{
//	int aniId = -1;
//	/*if (!isInGround)
//	{
//		if (abs(ax) == MARIO_ACCEL_RUN_X)
//		{
//			if (nx >= 0)
//				aniId = ID_ANI_MARIO_FIRE_JUMP_RUN_RIGHT;
//			else
//				aniId = ID_ANI_MARIO_FIRE_JUMP_RUN_LEFT;
//		}
//		else
//		{
//			if (nx >= 0)
//				aniId = ID_ANI_MARIO_FIRE_JUMP_WALK_RIGHT;
//			else
//				aniId = ID_ANI_MARIO_FIRE_JUMP_WALK_LEFT;
//		}
//	}
//	else*/
//		if (isSitting)
//		{
//			if (nx > 0)
//				aniId = ID_ANI_MARIO_FIRE_SIT_RIGHT;
//			else
//				aniId = ID_ANI_MARIO_FIRE_SIT_LEFT;
//		}
//		else
//			if (vx == 0)
//			{
//				if (nx > 0) aniId = MARIO_ANI_FIRE_IDLE;
//				else aniId = ID_ANI_MARIO_FIRE_IDLE_LEFT;
//				if (vy < 0)
//				{
//					if (nx < 0)
//					{
//						aniId = ID_ANI_MARIO_FIRE_JUMP_WALK_LEFT;
//					}
//					else
//						aniId = ID_ANI_MARIO_FIRE_JUMP_WALK_RIGHT;
//				}
//			}
//			else if (vx > 0)
//			{
//
//				if (nx > 0)
//				{
//					if (power_melter_stack == POWER_METER_FULL)
//					{
//						aniId = ID_ANI_MARIO_FIRE_RUNNING_RIGHT;
//					}
//					else
//						aniId = ID_ANI_MARIO_FIRE_WALKING_RIGHT;
//				}
//				else
//				{
//					aniId = ID_ANI_MARIO_FIRE_BRACE_RIGHT;
//				}
//
//				if (vy < 0)
//				{
//					aniId = ID_ANI_MARIO_FIRE_JUMP_WALK_RIGHT;
//				}
//				if (this->state == MARIO_STATE_KICK)
//					aniId = ID_ANI_MARIO_FIRE_KICK_RIGHT;
//
//			}
//			else // vx < 0
//			{
//				if (nx < 0)
//				{
//					if (power_melter_stack == POWER_METER_FULL)
//					{
//						aniId = ID_ANI_MARIO_FIRE_RUNNING_LEFT;
//					}
//					else
//						aniId = ID_ANI_MARIO_FIRE_WALKING_LEFT;
//				}
//				else
//				{
//					aniId = ID_ANI_MARIO_FIRE_BRACE_LEFT;
//				}
//
//				if (vy < 0)
//				{
//					aniId = ID_ANI_MARIO_FIRE_JUMP_WALK_LEFT;
//				}
//				if (this->state == MARIO_STATE_KICK)
//					aniId = ID_ANI_MARIO_FIRE_KICK_LEFT;
//
//			}
//
//	if (aniId == -1) aniId = MARIO_ANI_FIRE_IDLE;
//
//	return aniId;
//}
//
//
//
////
//// Get animdation ID for raccoon Mario
////
//int Mario::GetAniIdRaccoon()
//{
//	int aniId = -1;
//	/*if (!isInGround)
//	{
//		if (abs(ax) == MARIO_ACCEL_RUN_X)
//		{
//			if (nx >= 0)
//				aniId = ID_ANI_MARIO_RACCOON_JUMP_RUN_RIGHT;
//			else
//				aniId = ID_ANI_MARIO_RACCOON_JUMP_RUN_LEFT;
//		}
//		else
//		{
//			if (nx >= 0)
//				aniId = ID_ANI_MARIO_RACCOON_JUMP_WALK_RIGHT;
//			else
//				aniId = ID_ANI_MARIO_RACCOON_JUMP_WALK_LEFT;
//		}
//	}
//	else*/
//		if (isSitting)
//		{
//			if (nx > 0)
//				aniId = ID_ANI_MARIO_RACCOON_SIT_RIGHT;
//			else
//				aniId = ID_ANI_MARIO_RACCOON_SIT_LEFT;
//		}
//		else
//			if (vx == 0)
//			{
//				if (nx > 0) aniId = ID_ANI_MARIO_RACCOON_IDLE_RIGHT;
//				else aniId = ID_ANI_MARIO_RACCOON_IDLE_LEFT;
//				if (vy < 0)
//				{
//					if (nx < 0)
//					{
//						aniId = ID_ANI_MARIO_RACCOON_JUMP_WALK_LEFT;
//					}
//					else
//						aniId = ID_ANI_MARIO_RACCOON_JUMP_WALK_RIGHT;
//				}
//			}
//			else if (vx > 0)
//			{
//				if (nx > 0)
//				{
//					if (power_melter_stack == POWER_METER_FULL)
//					{
//						aniId = ID_ANI_MARIO_RACCOON_RUNNING_RIGHT;
//					}
//					else
//						aniId = ID_ANI_MARIO_RACCOON_WALKING_RIGHT;
//				}
//				else
//				{
//					aniId = ID_ANI_MARIO_RACCOON_BRACE_RIGHT;
//				}
//
//				if (vy < 0)
//				{
//					aniId = ID_ANI_MARIO_RACCOON_JUMP_WALK_RIGHT;
//				}
//				if (this->state == MARIO_STATE_KICK)
//					aniId = ID_ANI_MARIO_RACCOON_KICK_RIGHT;
//
//			}
//			else // vx < 0
//			{
//				if (nx < 0)
//				{
//					if (power_melter_stack == POWER_METER_FULL)
//					{
//						aniId = ID_ANI_MARIO_RACCOON_RUNNING_LEFT;
//					}
//					else
//						aniId = ID_ANI_MARIO_RACCOON_WALKING_LEFT;
//				}
//				else
//				{
//					aniId = ID_ANI_MARIO_RACCOON_BRACE_LEFT;
//				}
//
//				if (vy < 0)
//				{
//					aniId = ID_ANI_MARIO_RACCOON_JUMP_WALK_LEFT;
//				}
//				if (this->state == MARIO_STATE_KICK)
//					aniId = ID_ANI_MARIO_RACCOON_KICK_LEFT;
//
//			}
//
//	if (aniId == -1) aniId = ID_ANI_MARIO_RACCOON_IDLE_RIGHT;
//
//	return aniId;
//}