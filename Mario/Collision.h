#pragma once

#include <Windows.h>
#include <vector>
#include <algorithm>

using namespace std;

class GameObject;
typedef GameObject* LPGAMEOBJECT;

struct CollisionEvent;
typedef CollisionEvent* LPCOLLISIONEVENT;

struct CollisionEvent 
{
	LPGAMEOBJECT obj;
	float t, nx, ny;
	CollisionEvent(float t, float nx,
		float ny, LPGAMEOBJECT obj = NULL)
	{
		this->t = t;
		this->nx = nx;
		this->ny = ny;
		this->obj = obj;
	}

	static bool compare(const LPCOLLISIONEVENT& a,
		LPCOLLISIONEVENT& b)
	{
		return a->t < b->t;
	}
};

//class CCollision
//{
//	static CCollision* __instance;
//public: 
//	static void SweptAABB(
//		float ml,			// move left 
//		float mt,			// move top
//		float mr,			// move right 
//		float mb,			// move bottom
//		float dx,			// 
//		float dy,			// 
//		float sl,			// static left
//		float st,
//		float sr,
//		float sb,
//		float& t,
//		float& nx,
//		float& ny);
//
//	LPCOLLISIONEVENT SweptAABB(
//		LPGAMEOBJECT objSrc, 
//		DWORD dt,
//		LPGAMEOBJECT objDest); 
//	void Scan(
//		LPGAMEOBJECT objSrc, 
//		DWORD dt, 
//		vector<LPGAMEOBJECT>* objDests, 
//		vector<LPCOLLISIONEVENT>& coEvents);
//
//	void Filter(
//		LPGAMEOBJECT objSrc,
//		vector<LPCOLLISIONEVENT>& coEvents,
//		LPCOLLISIONEVENT &colX,
//		LPCOLLISIONEVENT &colY, 
//		int filterBlock,		
//		int filterX,
//		int filterY);
//
//	void Process(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* coObjects);
//
//	static CCollision* GetInstance();
//};