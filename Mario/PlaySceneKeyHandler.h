#pragma once

#include "Scene.h"

class PlaySceneKeyHandler: public SceneKeyHandler
{
public:
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	PlaySceneKeyHandler(LPSCENE s) :SceneKeyHandler(s) {};
};
