#pragma once

#include <Windows.h>
#include <unordered_map>

#include "Sprites.h"
#include "AnimationFrame.h"

using namespace std;

class Animation
{
	DWORD lastFrameTime;
	int defaultTime;
	int currentFrame;
	vector<LPANIMATION_FRAME> frames;
public:
	Animation(int defaultTime = 100) { this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = -1; }
	void Add(int spriteId, DWORD time = 0);
	void Render(int nx, float x, float y, int alpha = 255);
};

typedef Animation* LPANIMATION;