#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CMario* mario = (CMario *)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer(); 

	switch (KeyCode)
	{
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT);
		break;
	/*case DIK_S:
		mario->SetState(MARIO_STATE_JUMP);
		break;*/
	case DIK_SPACE:
		mario->StartJumping();
		break;
	case DIK_0:
		mario->SetState(MARIO_STATE_DIE);
		break;
	case DIK_1:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_2:
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_3:
		mario->SetLevel(MARIO_LEVEL_FIRE);
		break;
	case DIK_4:
		mario->SetLevel(MARIO_LEVEL_RACCOON);
		break;
	case DIK_I:
		mario->Information();
		break;
	case DIK_R: // reset
		mario->SetState(MARIO_STATE_IDLE);
		mario->SetLevel(MARIO_LEVEL_BIG);
		mario->SetPosition(150.0f, 0.0f);
		mario->SetSpeed(0, 0);
		break;
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (KeyCode)
	{
	/*case DIK_S:
		mario->SetState(MARIO_STATE_RELEASE_JUMP);
		break;*/
	case DIK_SPACE:
		mario->unJump();
		break;
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT_RELEASE);
		break;
	case DIK_LSHIFT:
		mario->isPickingUp = false;
		DebugOut(L"Release B");
		break;
	}
}

void CSampleKeyHandler::KeyState(BYTE *states)
{
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (mario->GetState() == MARIO_STATE_DIE) return;
	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (game->IsKeyDown(DIK_LSHIFT))
		{
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
			mario->FillUpPowerMelter();
		}
		else
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
		if (game->IsKeyDown(DIK_LEFT))
			mario->SetState(MARIO_STATE_BRAKE_RIGHT);
	}

	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (game->IsKeyDown(DIK_LSHIFT))
		{
			mario->SetState(MARIO_STATE_RUNNING_LEFT);
			mario->FillUpPowerMelter();
		}
		else
			mario->SetState(MARIO_STATE_WALKING_LEFT);
		if (game->IsKeyDown(DIK_RIGHT))
		{
			mario->SetState(MARIO_STATE_BRAKE_LEFT);
		}
	}
	else if (game->IsKeyDown(DIK_SPACE))
		mario->Jump();
	else if (game->IsKeyDown(DIK_LSHIFT))
		mario->PickUp();
	//	mario->pickUp = true;
	else
	{
		mario->SetState(MARIO_STATE_IDLE);
		mario->LosePowerMelter();
	}


	/*if (game->IsKeyDown(DIK_RIGHT))
	{
		if (game->IsKeyDown(DIK_A))
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
		else
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (game->IsKeyDown(DIK_A))
			mario->SetState(MARIO_STATE_RUNNING_LEFT);
		else
			mario->SetState(MARIO_STATE_WALKING_LEFT);
	}
	else
		mario->SetState(MARIO_STATE_IDLE);*/
}