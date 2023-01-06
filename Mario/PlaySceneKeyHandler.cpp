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
		mario->Float();
		mario->Fly();
		break;
	case DIK_U:
		mario->UpForm();
		break;
	case DIK_R:
		mario->Reset();
	case DIK_C:
	{
		KoopaTroopa* koopa = new KoopaTroopa(960, 383, 1);
		koopa->SetEnable();
		AnimationSets* animation_sets = AnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(4);
		koopa->SetAnimationSet(ani_set);
		((PlayScene*)scence)->AddObject((KoopaTroopa*)koopa);
		break;
	}
	case DIK_G:
	{
		Goomba* goopa = new Goomba(960, 383);
		goopa->SetEnable();
		AnimationSets* animation_sets = AnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(3);
		goopa->SetAnimationSet(ani_set);
		((PlayScene*)scence)->AddObject((Goomba*)goopa);
		break;
	}
	case DIK_I:
		mario->Information();
		break;
	case DIK_J:
		int flag = mario->Skill();

		if (flag == 1)
		{
			GameObject* fireBall = mario->ShootFireBall();
			((PlayScene*)scence)->AddObject((FireBall*)fireBall);
		}
		if (flag == 2)
		{
			mario->TailAttack();
		}

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
		
		mario->ReleaseJ();
		//mario->SetState(MARIO_STATE_IDLE);
		break;
	case DIK_A:
		mario->turnFriction = true;
		break;
	case DIK_D:
		mario->turnFriction = true;
		break;
	case DIK_K:
		/* mario->Float();*/
		mario->Jump();
		break;
	case DIK_S:
		mario->SetState(MARIO_STATE_IDLE);
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
		if (!mario->IsFlying() && !mario->IsFloating())
		{
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
	}
	if (game->IsKeyDown(DIK_A))
	{
		mario->SetDirect(false);
		if (!mario->IsFlying() && !mario->IsFloating())
		{
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
		/*	mario->Float();
	mario->Fly();*/
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