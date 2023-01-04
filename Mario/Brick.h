#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

class Brick : public GameObject {
public:
	Brick() {}
	Brick(float x, float y) : GameObject(x, y) {}
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b, bool isEnable);
};