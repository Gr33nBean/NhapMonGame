#include "PlaySceneKeyHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"

void PlaySceneKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	Mario* mario = ((PlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_K:
		mario->StartJumping();
		break;
	case DIK_U:
		mario->UpForm();
		break;
	case DIK_I:
		mario->Information();
		break;
	case DIK_J:
		mario->Skill();
		break;
		//case DIK_C:
		//	CreateKoopa();
		//	break;

	}
}

void PlaySceneKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	Mario* mario = ((PlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_J:
		mario->isPickingUp = false;
		mario->isPressedJ = false;
		mario->SetState(MARIO_STATE_IDLE);
		break;
	case DIK_A:
		mario->turnFriction = true;
		break;
	case DIK_D:
		mario->turnFriction = true;
		break;
	case DIK_K:
		mario->Jump();
		break;
	}
}

void PlaySceneKeyHandler::KeyState(BYTE *states)
{
	Game* game = Game::GetInstance();
	Mario* mario = ((PlayScene*)scence)->GetPlayer();

	// disable control key when Mario die 
	if (mario->GetState() == MARIO_STATE_DEATH) return;
	if (game->IsKeyDown(DIK_D))
	{
		mario->SetDirect(true);
		if (game->IsKeyDown(DIK_J))
		{
			mario->FillUpPowerMelter();
			mario->PickUp();
		}

		mario->SetState(MARIO_STATE_WALKING);
		if (game->IsKeyDown(DIK_A))
		{
			mario->SetDirect(false);
			mario->SetState(MARIO_STATE_BRAKING);
		}
		if (game->IsKeyDown(DIK_K))
		{
			mario->SuperJump();
		}
	}
	else if (game->IsKeyDown(DIK_A))
	{
		mario->SetDirect(false);
		if (game->IsKeyDown(DIK_J))
		{
			mario->FillUpPowerMelter();
			mario->PickUp();
		}
		mario->SetState(MARIO_STATE_WALKING);
		if (game->IsKeyDown(DIK_D))
		{
			mario->SetDirect(true);
			mario->SetState(MARIO_STATE_BRAKING);
		}
		if (game->IsKeyDown(DIK_K))
		{
			mario->SuperJump();
		}
	}
	//else if (game->IsKeyDown(DIK_K))
	//{
	//	/*mario->Jump();*/
	//}
	else if (game->IsKeyDown(DIK_S))
	{
		mario->Squat();
	}
	else if (game->IsKeyDown(DIK_K))
	{
		mario->SuperJump();
	}
	else
	{
		mario->LosePowerMelter();
		/*	mario->SetState(MARIO_STATE_IDLE);*/
		/*	mario->turnFriction = true;*/
		if (game->IsKeyDown(DIK_J))
		{
			mario->PickUp();
		}
	}

}