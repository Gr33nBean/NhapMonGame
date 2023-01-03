#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Enemy.h"
#include "Goomba.h"
#include "KoopaTroopa.h"
#include "Coin.h"
#include "Portal.h"

#include "Collision.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if (abs(vx) > abs(maxVx)) vx = maxVx;

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	isInGround = false;

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();
	else if (level == MARIO_LEVEL_FIRE)
		aniId = GetAniIdFire();
	else if (level == MARIO_LEVEL_RACCOON)
		aniId = GetAniIdRaccoon();

	animations->Get(aniId)->Render(x, y);

	//RenderBoundingBox();

	DebugOutTitle(L"Coins: %d", coin);
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return;

	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		vy = -MARIO_JUMP_SPEED_Y;
		/*if (isInGround)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_LONG_JUMP_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}*/
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;

	case MARIO_STATE_SIT:
		if (isInGround && level != MARIO_LEVEL_SMALL)
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
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;

	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_WALK_X + (BUFF_SPEED * power_melter_stack);
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -(MARIO_ACCEL_WALK_X + (BUFF_SPEED * power_melter_stack));
		nx = -1;
		break;
	case MARIO_STATE_BRAKE_LEFT:
		//vx = -(MARIO_BRAKE_DEFLECT_SPEED);
		ax = 0;
		nx = 1;
		break;
	case MARIO_STATE_BRAKE_RIGHT:
		//vx = MARIO_BRAKE_DEFLECT_SPEED;
		ax = 0;
		nx = -1;
		break;
	case MARIO_STATE_LONG_JUMP:
		vy = -MARIO_LONG_JUMP_SPEED_Y;
		break;
	case MARIO_STATE_KICK:
		break;
	}


	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom, bool isEnable)
{
	if (level == MARIO_LEVEL_BIG)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_BIG_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		left = x - MARIO_SMALL_BBOX_WIDTH / 2;
		top = y - MARIO_SMALL_BBOX_HEIGHT / 2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
	else if (level == MARIO_LEVEL_FIRE)
	{
		if (isSitting)
		{
			left = x - MARIO_FIRE_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_FIRE_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_FIRE_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_FIRE_SITTING_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_FIRE_BBOX_WIDTH / 2;
			top = y - MARIO_FIRE_BBOX_HEIGHT / 2;
			right = left + MARIO_FIRE_BBOX_WIDTH;
			bottom = top + MARIO_FIRE_BBOX_HEIGHT;
		}
		
	}
	else if (level == MARIO_LEVEL_RACCOON)
	{
		if (isSitting)
		{
			left = x - MARIO_RACCOON_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_RACCOON_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_RACCOON_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_RACCOON_SITTING_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_RACCOON_BBOX_WIDTH / 2;
			top = y - MARIO_RACCOON_BBOX_HEIGHT / 2;
			right = left + MARIO_RACCOON_BBOX_WIDTH;
			bottom = top + MARIO_RACCOON_BBOX_HEIGHT;
		}
		
	}
	

}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		switch (l)
		{
		case MARIO_LEVEL_BIG:
			y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
			break;
		case MARIO_LEVEL_FIRE:
			y -= (MARIO_FIRE_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
			break;
		case MARIO_LEVEL_RACCOON:
			y -= (MARIO_RACCOON_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
			break;
		}
		
	}
	level = l;
}

void CMario::Jump()
{
	DWORD current = GetTickCount64();
	if (current - long_jump_start > MARIO_LONG_JUMP_TIME && isInGround == true
		&& long_jump_start != 0)
	{
		// nếu Mario chưa tiếp đất hoặc vật thì ko cho phép nhảy
		this->SetState(MARIO_STATE_LONG_JUMP);
		isJump == false;
		long_jump_start = 0;
	}


}
void CMario::unJump()
{
	DWORD current = GetTickCount64();
	if (current - long_jump_start < MARIO_LONG_JUMP_TIME && this->isInGround == true && isJump == true)
	{
		// nếu Mario chưa tiếp đất hoặc vật thì ko cho phép nhảy
		// không cho phép nhảy ngắn khi Mario đã nhảy
		this->SetState(MARIO_STATE_JUMP);
		DebugOut(L"\nxyz");

	}
	else
	{
		DebugOut(L"\nisInGround %d", isInGround);
		DebugOut(L"\nisJump %d", isJump);
		DebugOut(L"\nC: %d - L: %d", current, long_jump_start);
	}
	isJump = false;
	long_jump_start = 0;
}

void CMario::FillUpPowerMelter()
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
void CMario::LosePowerMelter()
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

void CMario::Information()
{
	DebugOut(L"\nX: %d, Y: %d", this->x, this->y);
	DebugOut(L"\nVx: %f, Vy: %f", this->vx, this->vy);
}
void CMario::PickUp()
{
	isPickingUp = true;

}



// Collision
void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) isInGround = true;
	}
	else 
	if (e->nx != 0 && e->obj->IsBlocking() && isPickingUp == false)
	{
		vx = 0;
	}

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CKoopaTroopa*>(e->obj))
		OnCollisionWithKoopaTroopa(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

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
				if (level > MARIO_LEVEL_SMALL)
				{
					level -= 1;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}

void CMario::OnCollisionWithKoopaTroopa(LPCOLLISIONEVENT e)
{
	CKoopaTroopa* troopa = dynamic_cast<CKoopaTroopa*>(e->obj);

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
				if (level > MARIO_LEVEL_SMALL)
				{
					level -= 1;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
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

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	coin++;
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	/*if (!isInGround)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else*/
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_BIG_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_BIG_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
				if (vy < 0)
				{
					if (nx < 0)
					{
						aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
					}
					else
						aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
				}
			}
			else if (vx > 0)
			{
				if (nx > 0)
				{
					if (power_melter_stack == POWER_METER_FULL)
					{
						aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
					}
					else
						aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
				}
				else if (nx < 0)
				{
					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
				}
				if (vy < 0)
				{
					aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
				}
				if (this->state == MARIO_STATE_KICK)
					aniId = ID_ANI_MARIO_SMALL_KICK_RIGHT;
			}
			else if (vx < 0)
			{
				
				if (nx < 0)
				{
					if (power_melter_stack == POWER_METER_FULL)
					{
						aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
					}
					else
						aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
				}
				else
				{
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				}
				if (vy < 0)
				{
					aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
				}
				if (this->state == MARIO_STATE_KICK)
					aniId = ID_ANI_MARIO_SMALL_KICK_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;
	/*if (!isInGround)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_BIG_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_BIG_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_BIG_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_BIG_JUMP_WALK_LEFT;
		}
	}
	else*/
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_BIG_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_BIG_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_BIG_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_BIG_IDLE_LEFT;
				if (vy < 0)
				{
					if (nx < 0)
					{
						aniId = ID_ANI_MARIO_BIG_JUMP_WALK_LEFT;
					}
					else
						aniId = ID_ANI_MARIO_BIG_JUMP_WALK_RIGHT;
				}
			}
			else if (vx > 0)
			{

				if (nx > 0)
				{
					if (power_melter_stack == POWER_METER_FULL)
					{
						aniId = ID_ANI_MARIO_BIG_RUNNING_RIGHT;
					}
					else
						aniId = ID_ANI_MARIO_BIG_WALKING_RIGHT;
				}
				else if (nx < 0)
				{
					aniId = ID_ANI_MARIO_BIG_BRACE_RIGHT;
				}

				if (vy < 0)
				{
					aniId = ID_ANI_MARIO_BIG_JUMP_WALK_RIGHT;
				}
				if (this->state == MARIO_STATE_KICK)
					aniId = ID_ANI_MARIO_BIG_KICK_RIGHT;
			}
			else if (vx < 0)
			{
				if (nx < 0)
				{
					if (power_melter_stack == POWER_METER_FULL)
					{
						aniId = ID_ANI_MARIO_BIG_RUNNING_LEFT;
					}
					else
						aniId = ID_ANI_MARIO_BIG_WALKING_LEFT;
				}
				else
				{
					aniId = ID_ANI_MARIO_BIG_BRACE_LEFT;
				}

				if (vy < 0)
				{
					aniId = ID_ANI_MARIO_BIG_JUMP_WALK_LEFT;
				}
				if (this->state == MARIO_STATE_KICK)
					aniId = ID_ANI_MARIO_BIG_KICK_LEFT;

				/*if (ax > 0)
					aniId = ID_ANI_MARIO_BIG_BRACE_LEFT;
				else if ( power_melter_stack == POWER_METER_FULL)
					aniId = ID_ANI_MARIO_BIG_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_BIG_WALKING_LEFT;*/
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_BIG_IDLE_RIGHT;

	return aniId;
}


//
// Get animdation ID for fire Mario
//
int CMario::GetAniIdFire()
{
	int aniId = -1;
	/*if (!isInGround)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_FIRE_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_FIRE_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_FIRE_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_FIRE_JUMP_WALK_LEFT;
		}
	}
	else*/
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_FIRE_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_FIRE_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_FIRE_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_FIRE_IDLE_LEFT;
				if (vy < 0)
				{
					if (nx < 0)
					{
						aniId = ID_ANI_MARIO_FIRE_JUMP_WALK_LEFT;
					}
					else
						aniId = ID_ANI_MARIO_FIRE_JUMP_WALK_RIGHT;
				}
			}
			else if (vx > 0)
			{

				if (nx > 0)
				{
					if (power_melter_stack == POWER_METER_FULL)
					{
						aniId = ID_ANI_MARIO_FIRE_RUNNING_RIGHT;
					}
					else
						aniId = ID_ANI_MARIO_FIRE_WALKING_RIGHT;
				}
				else
				{
					aniId = ID_ANI_MARIO_FIRE_BRACE_RIGHT;
				}

				if (vy < 0)
				{
					aniId = ID_ANI_MARIO_FIRE_JUMP_WALK_RIGHT;
				}
				if (this->state == MARIO_STATE_KICK)
					aniId = ID_ANI_MARIO_FIRE_KICK_RIGHT;

			}
			else // vx < 0
			{
				if (nx < 0)
				{
					if (power_melter_stack == POWER_METER_FULL)
					{
						aniId = ID_ANI_MARIO_FIRE_RUNNING_LEFT;
					}
					else
						aniId = ID_ANI_MARIO_FIRE_WALKING_LEFT;
				}
				else
				{
					aniId = ID_ANI_MARIO_FIRE_BRACE_LEFT;
				}

				if (vy < 0)
				{
					aniId = ID_ANI_MARIO_FIRE_JUMP_WALK_LEFT;
				}
				if (this->state == MARIO_STATE_KICK)
					aniId = ID_ANI_MARIO_FIRE_KICK_LEFT;

			}

	if (aniId == -1) aniId = ID_ANI_MARIO_FIRE_IDLE_RIGHT;

	return aniId;
}



//
// Get animdation ID for raccoon Mario
//
int CMario::GetAniIdRaccoon()
{
	int aniId = -1;
	/*if (!isInGround)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_RACCOON_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACCOON_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_RACCOON_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACCOON_JUMP_WALK_LEFT;
		}
	}
	else*/
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_RACCOON_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACCOON_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_RACCOON_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_RACCOON_IDLE_LEFT;
				if (vy < 0)
				{
					if (nx < 0)
					{
						aniId = ID_ANI_MARIO_RACCOON_JUMP_WALK_LEFT;
					}
					else
						aniId = ID_ANI_MARIO_RACCOON_JUMP_WALK_RIGHT;
				}
			}
			else if (vx > 0)
			{
				if (nx > 0)
				{
					if (power_melter_stack == POWER_METER_FULL)
					{
						aniId = ID_ANI_MARIO_RACCOON_RUNNING_RIGHT;
					}
					else
						aniId = ID_ANI_MARIO_RACCOON_WALKING_RIGHT;
				}
				else
				{
					aniId = ID_ANI_MARIO_RACCOON_BRACE_RIGHT;
				}

				if (vy < 0)
				{
					aniId = ID_ANI_MARIO_RACCOON_JUMP_WALK_RIGHT;
				}
				if (this->state == MARIO_STATE_KICK)
					aniId = ID_ANI_MARIO_RACCOON_KICK_RIGHT;

			}
			else // vx < 0
			{
				if (nx < 0)
				{
					if (power_melter_stack == POWER_METER_FULL)
					{
						aniId = ID_ANI_MARIO_RACCOON_RUNNING_LEFT;
					}
					else
						aniId = ID_ANI_MARIO_RACCOON_WALKING_LEFT;
				}
				else
				{
					aniId = ID_ANI_MARIO_RACCOON_BRACE_LEFT;
				}

				if (vy < 0)
				{
					aniId = ID_ANI_MARIO_RACCOON_JUMP_WALK_LEFT;
				}
				if (this->state == MARIO_STATE_KICK)
					aniId = ID_ANI_MARIO_RACCOON_KICK_LEFT;

			}

	if (aniId == -1) aniId = ID_ANI_MARIO_RACCOON_IDLE_RIGHT;

	return aniId;
}