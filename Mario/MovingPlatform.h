#pragma once
#include "GameObject.h"
#include "Platform.h"
#include <algorithm>


// 
// The most popular type of object in Mario! 
// 
class CMovingPlatform : public CPlatform
{
protected:
	int length;				// Unit: cell 
	float cellWidth;
	float cellHeight;
	int spriteIdBegin, spriteIdMiddle, spriteIdEnd;
	float top, bottom;

public:
	CMovingPlatform(float x, float y,
		float cell_width, float cell_height, int length,
		int sprite_id_begin, int sprite_id_middle, int sprite_id_end, float top, float bottom): CPlatform( x,  y, cell_width,  cell_height,  length, sprite_id_begin,  sprite_id_middle,  sprite_id_end)
	{
		this->top = top;
		this->bottom = bottom;
		vx = 0;
		vy = 0.1;
	}

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
	{
		if (y<top || y> bottom)
		{
			vy = -vy;
			if (y<top)
			{
				y = top;
			}
			else
			{
				y = bottom;
			}
		}
		y += vy * dt;
	}

};

typedef CPlatform* LPPLATFORM;