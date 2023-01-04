#include "Coin.h"

void CCoin::Render()
{
	animation_set->at(0)->Render(-1, x, y);
	//RenderBoundingBox();
}

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b, bool isEnable)
{
	l = x;
	t = y;
	r = x + COIN_BBOX_WIDTH;
	b = y + COIN_BBOX_HEIGHT;
}