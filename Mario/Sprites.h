#pragma once
#include <Windows.h>
#include <unordered_map>

#include <d3dx10.h>

#include "Texture.h"
#include "Sprite.h"

using namespace std;

/*
	Manage sprite database
*/
class Sprites
{
	static Sprites* __instance;

	unordered_map<int, LPSPRITE> sprites;

public:
	void Add(int id, int left, int top, int right, int bottom, LPTEXTURE tex);
	LPSPRITE Get(int id);
	LPSPRITE& operator[](int id) { return sprites[id]; }
	void Clear();

	static Sprites* GetInstance();
};

