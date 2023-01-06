#pragma once
#include"GameObject.h"
#include"ThongSo.h"

class Ground : public GameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b, bool isEnable);
};