#include "PlaySceneKeyHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"

void PlaySceneKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	Mario* mario = (Mario *)((LPPLAYSCENE)Game::GetInstance()->GetCurrentScene())->GetPlayer(); 

	switch (KeyCode)
	{
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT);
		break;
	case DIK_K:
		mario->SetState(MARIO_STATE_JUMPING);
		break;
	/*case DIK_SPACE:
		mario->StartJumping();
		break;*/
	case DIK_0:
		mario->SetState(MARIO_STATE_DEATH);
		break;
	case DIK_U:
		mario->UpForm();
		break;
	case DIK_I:
		mario->Information();
		break;
	//case DIK_R: // reset
	//	mario->SetState(MARIO_STATE_IDLE);
	//	mario->SetLevel(MARIO_BIG_FORM);
	//	mario->SetPosition(150.0f, 0.0f);
	//	mario->SetSpeed(0, 0);
	//	break;
	}
}

void PlaySceneKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	Mario* mario = (Mario*)((LPPLAYSCENE)Game::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (KeyCode)
	{
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT_RELEASE);
		break;
	}
}

void PlaySceneKeyHandler::KeyState(BYTE *states)
{
	LPGAME game = Game::GetInstance();
	Mario* mario = (Mario*)((LPPLAYSCENE)Game::GetInstance()->GetCurrentScene())->GetPlayer();

	if (mario->GetState() == MARIO_STATE_DEATH) return;
	if (game->IsKeyDown(DIK_D))
	{

		mario->SetDirect(true);
		mario->SetState(MARIO_STATE_WALKING);

	}
	else if (game->IsKeyDown(DIK_A))
	{
		mario->SetDirect(false);
		mario->SetState(MARIO_STATE_WALKING);

	}
	//else if (game->IsKeyDown(DIK_K))
	//{
	//	/*mario->Jump();*/
	//}
	else
	{
		mario->SetState(MARIO_STATE_IDLE);
	}

}