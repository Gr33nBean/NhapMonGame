#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"

// ---- Thông số ----
#define MARIO_ACCEL_WALK_X	0.0005f
#define MARIO_ACCEL_RUN_X	0.0007f

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_RUNNING_SPEED		0.2f

#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_LONG_JUMP_SPEED_Y	0.6f

#define MARIO_JUMP_DEFLECT_SPEED  0.4f
#define MARIO_BRAKE_DEFLECT_SPEED 0.05f 

#define MARIO_GRAVITY			0.002f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f 

#define BUFF_SPEED		0.02f //tốc độ tăng lên khi tích stack
#define POWER_METER_FULL 7 // số stack tối đa
#define STACK_TIME 200 // thời gian tích 1 stack


// ---- State ----

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_DIE				400
#define MARIO_STATE_LONG_JUMP		500
#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_RUNNING_RIGHT	600
#define MARIO_STATE_RUNNING_LEFT	700

#define MARIO_STATE_BRAKE_RIGHT		800
#define MARIO_STATE_BRAKE_LEFT		900

#define MARIO_STATE_SIT				1000
#define MARIO_STATE_SIT_RELEASE		1001


#pragma region ANIMATION_ID

// BIG MARIO
#define ID_ANI_MARIO_BIG_IDLE_RIGHT 400
#define ID_ANI_MARIO_BIG_IDLE_LEFT 401

#define ID_ANI_MARIO_BIG_WALKING_RIGHT 500
#define ID_ANI_MARIO_BIG_WALKING_LEFT 501

#define ID_ANI_MARIO_BIG_RUNNING_RIGHT 600
#define ID_ANI_MARIO_BIG_RUNNING_LEFT 601

#define ID_ANI_MARIO_BIG_JUMP_WALK_RIGHT 700
#define ID_ANI_MARIO_BIG_JUMP_WALK_LEFT 701

#define ID_ANI_MARIO_BIG_JUMP_RUN_RIGHT 800
#define ID_ANI_MARIO_BIG_JUMP_RUN_LEFT 801

#define ID_ANI_MARIO_BIG_SIT_RIGHT 900
#define ID_ANI_MARIO_BIG_SIT_LEFT 901

#define ID_ANI_MARIO_BIG_BRACE_RIGHT 1000
#define ID_ANI_MARIO_BIG_BRACE_LEFT 1001

#define ID_ANI_MARIO_DIE 999

// SMALL MARIO
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT 1100
#define ID_ANI_MARIO_SMALL_IDLE_LEFT 1102

#define ID_ANI_MARIO_SMALL_WALKING_RIGHT 1200
#define ID_ANI_MARIO_SMALL_WALKING_LEFT 1201

#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT 1300
#define ID_ANI_MARIO_SMALL_RUNNING_LEFT 1301

#define ID_ANI_MARIO_SMALL_BRACE_RIGHT 1400
#define ID_ANI_MARIO_SMALL_BRACE_LEFT 1401

#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT 1500
#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT 1501

#define ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT 1600
#define ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT 1601


// FIRE MARIO
#define ID_ANI_MARIO_FIRE_IDLE_RIGHT 2100
#define ID_ANI_MARIO_FIRE_IDLE_LEFT 2102

#define ID_ANI_MARIO_FIRE_WALKING_RIGHT 2200
#define ID_ANI_MARIO_FIRE_WALKING_LEFT 2201

#define ID_ANI_MARIO_FIRE_RUNNING_RIGHT 2300
#define ID_ANI_MARIO_FIRE_RUNNING_LEFT 2301

#define ID_ANI_MARIO_FIRE_JUMP_WALK_RIGHT 2400
#define ID_ANI_MARIO_FIRE_JUMP_WALK_LEFT 2401

#define ID_ANI_MARIO_FIRE_JUMP_RUN_RIGHT 2500
#define ID_ANI_MARIO_FIRE_JUMP_RUN_LEFT 2501

#define ID_ANI_MARIO_FIRE_SIT_RIGHT 2600
#define ID_ANI_MARIO_FIRE_SIT_LEFT 2601

#define ID_ANI_MARIO_FIRE_BRACE_RIGHT 2700
#define ID_ANI_MARIO_FIRE_BRACE_LEFT 2701

// RACCOON MARIO
#define ID_ANI_MARIO_RACCOON_IDLE_RIGHT 3100
#define ID_ANI_MARIO_RACCOON_IDLE_LEFT 3102

#define ID_ANI_MARIO_RACCOON_WALKING_RIGHT 3200
#define ID_ANI_MARIO_RACCOON_WALKING_LEFT 3201

#define ID_ANI_MARIO_RACCOON_RUNNING_RIGHT 3300
#define ID_ANI_MARIO_RACCOON_RUNNING_LEFT 3301

#define ID_ANI_MARIO_RACCOON_JUMP_WALK_RIGHT 3400
#define ID_ANI_MARIO_RACCOON_JUMP_WALK_LEFT 3401

#define ID_ANI_MARIO_RACCOON_JUMP_RUN_RIGHT 3500
#define ID_ANI_MARIO_RACCOON_JUMP_RUN_LEFT 3501

#define ID_ANI_MARIO_RACCOON_SIT_RIGHT 3600
#define ID_ANI_MARIO_RACCOON_SIT_LEFT 3601

#define ID_ANI_MARIO_RACCOON_BRACE_RIGHT 3700
#define ID_ANI_MARIO_RACCOON_BRACE_LEFT 3701
#pragma endregion

#define GROUND_Y 160.0f


#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_FIRE	3
#define MARIO_LEVEL_RACCOON	4


#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 24
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 12

#define MARIO_FIRE_BBOX_WIDTH 15
#define MARIO_FIRE_BBOX_HEIGHT 27
#define MARIO_FIRE_SITTING_BBOX_WIDTH  15
#define MARIO_FIRE_SITTING_BBOX_HEIGHT 17

#define MARIO_RACCOON_BBOX_WIDTH 23
#define MARIO_RACCOON_BBOX_HEIGHT 28
#define MARIO_RACCOON_SITTING_BBOX_WIDTH  20
#define MARIO_RACCOON_SITTING_BBOX_HEIGHT 18


#define MARIO_UNTOUCHABLE_TIME 1000
#define MARIO_LONG_JUMP_TIME 200

class CMario : public CGameObject
{
	int level;
	int untouchable;
	ULONGLONG untouchable_start;
	BOOLEAN isInGround;

	BOOLEAN isSitting;
	float maxVx;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 

	DWORD stack_time_start;
	int power_melter_stack;
	int jump_stack;
	DWORD long_jump_start; // tính khi nhấn giữ nút tính stack


	int coin; 

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopaTroopa(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);

	int GetAniIdBig();
	int GetAniIdSmall();
	int GetAniIdFire();
	int GetAniIdRaccoon();

public:
	bool isJump;
	bool isPickingUp;
	CMario(float x, float y) : CGameObject(x, y)
	{
		level = MARIO_LEVEL_RACCOON;
		untouchable = 0;
		untouchable_start = -1;
		isInGround = false;

		isSitting = false;
		maxVx = 0.0f;
		ax = 0.0f;
		ay = MARIO_GRAVITY; 

		coin = 0;


		isJump = false;
		power_melter_stack = 1;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	void SetLevel(int l);
	

	int IsCollidable()
	{ 
		return (state != MARIO_STATE_DIE); 
	}

	int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable==0); }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }

	void StartJumping() { long_jump_start = GetTickCount64(); isInGround = false; isJump = true; }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void Jump();
	void unJump();
	void FillUpPowerMelter();
	void LosePowerMelter();
	void Information();
	void PickUp();
};