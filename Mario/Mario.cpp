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
#include "FireBall.h"
#include "Collision.h"


#pragma region Các hàm cập nhật tọa độ, animation
void Mario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	GameObject::Update(dt);
	// fall down slower 
	if (isInGround == false)
	{
		if (vy > -MARIO_FALLING_SPEED &&
			vy < MARIO_FALLING_SPEED)
			vy += MARIO_LOWER_GRAVITY * dt;
		else
			vy += MARIO_GRAVITY * dt;
	}
	else
		vy += MARIO_GRAVITY * dt;
	
	Friction();
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	// turn off collision when die 
	if (state != MARIO_STATE_DEATH)
		CalcPotentialCollisions(coObjects, coEvents);
	// reset untouchable timer if untouchable time has passed
	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		/*isInGround = false;*/
		if (vy > 0.2)
		{
			isInGround = false;
		}
	}
	else
	{
		float min_tx, min_ty, nex = 0, ney;

		FilterCollision(coEvents, coEventsResult,
			min_tx, min_ty,
			nex, ney);
		float x0 = x, y0 = y;
		x = x0 + dx;
		y = y0 + dy;
		// Collision logic 
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Enemy*>(e->obj))
			{
				Enemy* enemy = dynamic_cast<Enemy*>(e->obj);
				if (e->ny != 0)
				{
					if (e->ny < 0)
					{
						if (enemy->IsDead() != true)
						{
							enemy->SetBeingStromped();
							isInGround = true;
							isFloating = false;
							vy = -MARIO_JUMP_DEFLECT_SPEED;
							vx = nx * MARIO_WALK_DEFELCT_SPEED;
						}
						else
						{
							HandleCollision(min_tx, min_ty,
								nex, ney,
								x0, y0);
						}
					}

				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (enemy->IsDead() != true)
						{
							if (form > MARIO_SMALL_FORM)
							{
								form -= 1;
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DEATH);
						}
						else if (dynamic_cast<KoopaTroopa*>(enemy))
						{
							if (dynamic_cast<KoopaTroopa*>(enemy)->state ==
								KOOPATROOPA_STATE_HIDING)
							{
								if (isPressedJ == true)
								{
									dynamic_cast<KoopaTroopa*>(enemy)
										->PickUpBy(this);
									isPickingUp = true;
								}
								else if (isPressedJ == false)
								{
									HandleCollision(min_tx, min_ty,
										e->nx, e->ny,
										x0, y0);
									dynamic_cast<KoopaTroopa*>(enemy)->
										IsKicked(this->nx);
									this->SetState(MARIO_STATE_KICK);
								}
							}

						}
					}
				}
			}
			else if (!dynamic_cast<InvisibleBrick*>(e->obj) &&
				!dynamic_cast<Block*>(e->obj))
			{

				HandleCollision(min_tx, min_ty,
					e->nx, e->ny,
					x0, y0);
			}
			else if (dynamic_cast<Block*>(e->obj))
			{
				if (e->ny < 0)
				{
					HandleCollision(min_tx, min_ty,
						e->nx, e->ny,
						x0, y0);
					vy = 0;
				}
				else
				{
					x = x0 + dx;
					y = y0 + dy;
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	//DebugOut(L"\nState: %d", state);
}
void Mario::Render()
{
	int ani = -1;
	if (vx == 0)
	{
		if (!isPickingUp)
		{
			switch (form)
			{
			case MARIO_SMALL_FORM:
				ani = MARIO_ANI_SMALL_IDLE;
				break;
			case MARIO_BIG_FORM:
				ani = MARIO_ANI_BIG_IDLE;
				break;
			case MARIO_FIRE_FORM:
				ani = MARIO_ANI_FIRE_IDLE;
				break;
			case MARIO_RACCOON_FORM:
				ani = MARIO_ANI_RACCOON_IDLE;
				break;
			}
		}
		else
		{
			switch (form)
			{
			case MARIO_SMALL_FORM:
				ani = MARIO_ANI_SMALL_HOLD_SHELL_IDLE;
				break;
			case MARIO_BIG_FORM:
				ani = MARIO_ANI_BIG_HOLD_SHELL_IDLE;
				break;
			case MARIO_FIRE_FORM:
				ani = MARIO_ANI_FIRE_HOLD_SHELL_IDLE;
				break;
			case MARIO_RACCOON_FORM:
				ani = MARIO_ANI_RACCOON_HOLD_SHELL_IDLE;
				break;
			}
		}
	}
	if (vx != 0 && isInGround == true)
	{
		if (isPickingUp == false)
		{
			if (power_melter_stack < POWER_MELTER_BUFF_SPEED_LEVEL)
			{
				switch (form)
				{

				case MARIO_SMALL_FORM:
					ani = MARIO_ANI_SMALL_WALKING;
					break;
				case MARIO_BIG_FORM:
					ani = MARIO_ANI_BIG_WALKING;
					break;
				case MARIO_FIRE_FORM:
					ani = MARIO_ANI_FIRE_WALKING;
					break;
				case MARIO_RACCOON_FORM:
					ani = MARIO_ANI_RACCOON_WALKING;
					break;
				}
			}
			else if (power_melter_stack >= POWER_MELTER_BUFF_SPEED_LEVEL
				&& power_melter_stack < POWER_METER_FULL)
			{
				switch (form)
				{
				case MARIO_SMALL_FORM:
					ani = MARIO_ANI_SMALL_RUNNING;
					break;
				case MARIO_BIG_FORM:
					ani = MARIO_ANI_BIG_RUNNING;
					break;
				case MARIO_FIRE_FORM:
					ani = MARIO_ANI_FIRE_RUNNING;
					break;
				case MARIO_RACCOON_FORM:
					ani = MARIO_ANI_RACCOON_RUNNING;
					break;
				}
			}
			else if (power_melter_stack == POWER_METER_FULL)
			{
				switch (form)
				{
				case MARIO_SMALL_FORM:
					ani = MARIO_ANI_SMALL_FULL_RUNNING;
					break;
				case MARIO_BIG_FORM:
					ani = MARIO_ANI_BIG_FULL_RUNNING;
					break;
				case MARIO_FIRE_FORM:
					ani = MARIO_ANI_FIRE_FULL_RUNNING;
					break;
				case MARIO_RACCOON_FORM:
					ani = MARIO_ANI_RACCOON_FULL_RUNNING;
					break;
				}
			}
			if (state == MARIO_STATE_BRAKING)
			{
				switch (form)
				{
				case MARIO_SMALL_FORM:
					ani = MARIO_ANI_SMALL_FULL_BRAKING;
					break;
				case MARIO_BIG_FORM:
					ani = MARIO_ANI_BIG_FULL_BRAKING;
					break;
				case MARIO_FIRE_FORM:
					ani = MARIO_ANI_FIRE_FULL_BRAKING;
					break;
				case MARIO_RACCOON_FORM:
					ani = MARIO_ANI_RACCOON_FULL_BRAKING;
					break;
				}
			}
		}
		else
		{
			switch (form)
			{
			case MARIO_SMALL_FORM:
				ani = MARIO_ANI_SMALL_HOLD_SHELL_RUNNING;
				break;
			case MARIO_BIG_FORM:
				ani = MARIO_ANI_BIG_HOLD_SHELL_RUNNING;
				break;
			case MARIO_FIRE_FORM:
				ani = MARIO_ANI_FIRE_HOLD_SHELL_RUNNING;
				break;
			case MARIO_RACCOON_FORM:
				ani = MARIO_ANI_RACCOON_HOLD_SHELL_RUNNING;
				break;
			}
		}

	}
	if (isInGround == false && isFlying == false)
	{
		if (!isPickingUp)
		{
			if (power_melter_stack >= POWER_MELTER_BUFF_SPEED_LEVEL)
			{
				switch (form)
				{
				case MARIO_SMALL_FORM:
					ani = MARIO_ANI_SMALL_LONG_JUMPING;
					break;
				case MARIO_BIG_FORM:
					ani = MARIO_ANI_BIG_LONG_JUMPING;
					break;
				case MARIO_FIRE_FORM:
					ani = MARIO_ANI_FIRE_LONG_JUMPING;
					break;
				case MARIO_RACCOON_FORM:
					ani = MARIO_ANI_RACCOON_LONG_JUMPING;
					break;
				}
			}
			else
			{
				switch (form)
				{
				case MARIO_SMALL_FORM:
					ani = MARIO_ANI_SMALL_JUMPING;
					break;
				case MARIO_BIG_FORM:
					ani = MARIO_ANI_BIG_JUMPING;
					break;
				case MARIO_FIRE_FORM:
					ani = MARIO_ANI_FIRE_JUMPING;
					break;
				case MARIO_RACCOON_FORM:
					ani = MARIO_ANI_RACCOON_JUMPING;
					break;
				}
			}
		}
		else
		{
			switch (form)
			{
			case MARIO_SMALL_FORM:
				ani = MARIO_ANI_SMALL_HOLD_SHELL_RUNNING;
				break;
			case MARIO_BIG_FORM:
				ani = MARIO_ANI_BIG_HOLD_SHELL_RUNNING;
				break;
			case MARIO_FIRE_FORM:
				ani = MARIO_ANI_FIRE_HOLD_SHELL_RUNNING;
				break;
			case MARIO_RACCOON_FORM:
				ani = MARIO_ANI_RACCOON_HOLD_SHELL_RUNNING;
				break;
			}
		}
	}
	if (state == MARIO_STATE_SQUAT)
	{
		switch (form)
		{
		case MARIO_BIG_FORM:
			ani = MARIO_ANI_BIG_SQUAT;
			break;
		case MARIO_FIRE_FORM:
			ani = MARIO_ANI_FIRE_SQUAT;
			break;
		case MARIO_RACCOON_FORM:
			ani = MARIO_ANI_RACCOON_SQUAT;
			break;
		}
	}
	if (state == MARIO_STATE_KICK)
	{
		switch (form)
		{
		case MARIO_SMALL_FORM:
			ani = MARIO_ANI_SMALL_KICK_SHELL;
			break;
		case MARIO_BIG_FORM:
			ani = MARIO_ANI_BIG_KICK_SHELL;
			break;
		case MARIO_FIRE_FORM:
			ani = MARIO_ANI_FIRE_KICK_SHELL;
			break;
		case MARIO_RACCOON_FORM:
			ani = MARIO_ANI_RACCOON_KICK_SHELL;
			break;
		}
	}
	if (state == MARIO_STATE_FLOATING)
	{
		ani = MARIO_ANI_FLOATING;
	}
	if (state == MARIO_STATE_DEATH)
	{
		ani = MARIO_ANI_DIE;
	}
	if (state == MARIO_STATE_FLYING)
	{
		ani = MARIO_ANI_FLYING;
	}
	else if (state == MARIO_STATE_TAILATTACK)
		ani = MARIO_ANI_TAILATTACK;
	else if (state == MARIO_STATE_SHOOT_FIREBALL)
		ani = MARIO_ANI_SHOOT_FIRE_BALL;
	int alpha = 255;
	//DebugOut(L"Ani: %d\n", ani);
	if (untouchable) alpha = 128;
	animation_set->at(ani)->Render(nx, x, y, alpha);
	//RenderBoundingBox();
}
void Mario::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_WALKING:
		vx = (MARIO_WALKING_SPEED +
			(BUFF_SPEED * power_melter_stack)) * nx;
		break;
	case MARIO_STATE_JUMPING:
		isInGround = false;
		vy = -MARIO_JUMP_SPEED_Y;
		break;
	case MARIO_STATE_SUPER_JUMPING:
		isInGround = false;
		vy = -MARIO_SUPER_JUMP_SPEED;
		break;
	case MARIO_STATE_IDLE:
		vx = 0;
		isKickShell = false;
		break;
	case MARIO_STATE_DEATH:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	case MARIO_STATE_RUNNING:
		vx = (MARIO_WALKING_SPEED +
			(BUFF_SPEED * power_melter_stack)) * nx;
		break;
	case MARIO_STATE_BRAKING:
		vx = MARIO_BRAKE_DEFLECT_SPEED * -nx;
		break;
	case MARIO_STATE_STOP:
		vx -= 0.01 * vx;
		break;
	case MARIO_STATE_KICK:
		isKickShell = true;
		break;
	case MARIO_STATE_FLOATING:
		this->vy = MARIO_FLOATING_SPEED_Y;
		break;
	case MARIO_STATE_FLYING:
		this->vy = -MARIO_SUPER_JUMP_SPEED;
		this->vx = (MARIO_WALKING_SPEED +
			(BUFF_SPEED * power_melter_stack)) * nx;
		break;
	}
}
void Mario::GetBoundingBox(float& left, float& top, float& right, float& bottom, bool isEnable)
{
	left = x;
	if (state == MARIO_STATE_SQUAT)
	{
		top = y + this->GetHeight() - MARIO_BBOX_SQUAT_HEIGHT;
		right = x + MARIO_BBOX_SQUAT_WIDTH;
		bottom = top + MARIO_BBOX_SQUAT_HEIGHT;
	}
	else
	{
		top = y;
		if (form == MARIO_BIG_FORM)
		{
			right = x + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
		else if (form == MARIO_SMALL_FORM)
		{
			right = x + MARIO_SMALL_BBOX_WIDTH;
			bottom = top + MARIO_SMALL_BBOX_HEIGHT;
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
}
#pragma endregion

#pragma region Các hành động của MARIO
void Mario::FillUpPowerMelter()// Tăng stack năng lượng của Mario
{
	DWORD current = GetTickCount64();
	if (state != MARIO_STATE_FLYING &&
		isInGround != false)
	{
		if (stack_time_start == 0)
		{
			stack_time_start = current;
		}
		else
		{
			if (current - stack_time_start > STACK_TIME
				&& power_melter_stack < POWER_METER_FULL)
			{
				power_melter_stack += 1;
				stack_time_start = 0;
			}
		}
	}
}
void Mario::LosePowerMelter()// Power Stack sẽ cạn theo thời gian
{
	if (power_melter_stack > 0
		&& state != MARIO_STATE_FLYING)
	{
		DWORD current = GetTickCount64();
		if (stack_time_start == 0)
		{
			stack_time_start = current;
		}
		else
		{
			if (current - stack_time_start > LOSE_STACK_TIME
				&& power_melter_stack > POWER_MELTER_MINIMUM)
			{
				power_melter_stack -= 1;
				stack_time_start = 0;
			}
		}
	}
}
void Mario::PickUp()
{
	isPressedJ = true;
}
void Mario::SetDirect(bool nx)
{
	if (nx == true)
		this->nx = 1;
	else
		this->nx = -1;
}
void Mario::SuperJump()
{
	DWORD current = GetTickCount64();
	if (current - jump_time_start > MARIO_SUPER_JUMP_TIME
		&& isInGround == true
		&& jump_time_start != 0)
	{
		this->SetState(MARIO_STATE_SUPER_JUMPING);
		jump_time_start = 0;
	}
}
void Mario::Jump()
{
	DWORD current = GetTickCount64();
	if (current - jump_time_start < MARIO_SUPER_JUMP_TIME &&
		isInGround == true &&
		jump_time_start != 0)
	{
		this->SetState(MARIO_STATE_JUMPING);
		jump_time_start = 0;
		/*	DebugOut(L"Is Jumping\n");*/
	}
}
void Mario::Squat()
{
	if (form != MARIO_SMALL_FORM)
	{
		this->SetState(MARIO_STATE_SQUAT);
	}
}
int Mario::Skill()
{
	if (form == MARIO_FIRE_FORM)
	{
		return MARIO_SKILL_FIREBALL;
	}
	if (form == MARIO_RACCOON_FORM)
	{
		return MARIO_SKILL_TAILATTCK;
	}
	else
		return MARIO_DO_NOTHING;
}
void Mario::Friction()
{

	if (turnFriction == true)
	{
		this->vx -= this->vx * MARIO_FRICTION;
		if (abs(this->vx) < 0.007)
		{
			this->SetState(MARIO_STATE_IDLE);
			turnFriction = false;
		}

	}
}
GameObject* Mario::ShootFireBall()
{
	this->SetState(MARIO_STATE_SHOOT_FIREBALL);
	FireBall* fireBall;
	if (nx > 0)
		fireBall = new FireBall(this->x + MARIO_FIRE_BBOX_WIDTH,
			this->y + MARIO_FIRE_BBOX_WIDTH / 3,
			this->nx);
	else
		fireBall = new FireBall(this->x - MARIO_FIRE_BBOX_WIDTH,
			this->y + MARIO_FIRE_BBOX_WIDTH / 3,
			this->nx);
	return fireBall;
}
void Mario::Float()
{
	if (this->form == MARIO_RACCOON_FORM &&
		this->isInGround == false &&
		vy > 0 && isFlying == false)
	{
		this->SetState(MARIO_STATE_FLOATING);
		isFloating = true;
		/*	DebugOut(L"Is Floating\n");*/
	}
}
void Mario::TailAttack()
{
	this->nx = -this->nx;
	this->SetState(MARIO_STATE_TAILATTACK);
}
void Mario::Fly()
{

	if (form == MARIO_RACCOON_FORM)
	{
		DWORD current = GetTickCount64();
		/*	DebugOut(L"Power melter stack: %d\n", power_melter_stack);*/
		if (power_melter_stack == POWER_METER_FULL &&
			current - fly_time_start < MARIO_FLYING_LIMITED_TIME)
		{
			this->SetState(MARIO_STATE_FLYING);
			/*DebugOut(L"Is Flying\n");*/
			isFlying = true;

		}
		else if (current - fly_time_start >
			MARIO_FLYING_LIMITED_TIME
			&& fly_time_start != 0)
		{
			fly_time_start = 0;
			power_melter_stack = 0;
			isFlying = false;
		}
		else if (fly_time_start == 0 &&
			power_melter_stack == POWER_METER_FULL)
		{
			fly_time_start = current;
			this->SetState(MARIO_STATE_FLYING);
			isInGround = false;
		}
	}
}
#pragma endregion



void Mario::HandleCollision(float min_tx, float min_ty,
	float nex, float ney, float x0, float y0)
{
	if (nex != 0)
	{
		this->vx = 0;
		this->x = x0 + min_tx * this->dx + nex * 0.1f;
	}
	if (ney != 0)
	{
		this->vy = 0;
		this->y = y0 + min_ty * this->dy + ney * 0.1f;
		if (ney == -1)
		{
			isInGround = true;
			isFloating = false;
			this->ny = 0;
		}

	}
}
void Mario::UpForm()
{
	int diffy = 0;
	if (form == MARIO_SMALL_FORM)
	{
		diffy = MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT;

	}
	form += 1;
	if (form > MARIO_RACCOON_FORM)
	{
		diffy = MARIO_SMALL_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT;
		form = 0;
	}
	y -= diffy;
}
void Mario::Information()
{
	Game* game = Game::GetInstance();
	DebugOut(L"\nMario x: %f, y: %f ", x, y);
	DebugOut(L"Get Cam X: %f", game->GetCamX());
	//DebugOut(L"\nState %d", state);
}
int  Mario::GetWidth()
{
	if (form == MARIO_RACCOON_FORM)
		return MARIO_RACCOON_BBOX_WIDTH;
	else
		return MARIO_SMALL_BBOX_WIDTH;
}
int Mario::GetHeight()
{
	if (form == MARIO_SMALL_FORM)
		return MARIO_SMALL_BBOX_HEIGHT;
	else
		return MARIO_BIG_BBOX_HEIGHT;
}
Mario::Mario(float x, float y)
{
	this->x = x;
	this->y = y;
	vx = vy = 0;
	nx = 1;
	power_melter_stack = 0;
	form = MARIO_SMALL_FORM;
	isEnable = true;
	isKickShell = false;
	isPressedJ = false;
	isInGround = true;
}
void Mario::ReleaseJ()
{
	isPickingUp = false;
	isPressedJ = false;
	if (state == MARIO_STATE_SHOOT_FIREBALL ||
		state == MARIO_STATE_TAILATTACK)
		SetState(MARIO_STATE_IDLE);
}
void Mario::PressK()
{

}
void Mario::Reset()
{
	SetPosition(16, 400);
	vx = vy = 0;
	nx = 1;
	power_melter_stack = 0;
	form = MARIO_SMALL_FORM;
	isEnable = true;
	isKickShell = false;
	isPressedJ = false;
	isInGround = true;
}
bool Mario::IsFlying()
{
	return isFlying;
}
bool Mario::IsFloating()
{
	return isFloating;
}
bool Mario::IsInGround()
{
	return isInGround;
}