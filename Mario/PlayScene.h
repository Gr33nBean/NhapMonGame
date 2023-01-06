#pragma once
#include "Game.h"
#include "Textures.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "KoopaTroopa.h"
#include "Scene.h"
#include "Map.h"
#include "Block.h"
#include "Ground.h"
#include "Pipe.h"
#include"FireBall.h"

class PlayScene: public Scene
{
protected: 
	// A play scene has to have player, right? 
	Mario* player;					

	vector<LPGAMEOBJECT> objects;

	Map* map;
	bool _turnCamY;
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_ANIMATION_SETS(string line);

	void _ParseSection_MAPS(string line);
	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);

	void LoadAssets(LPCWSTR assetFile);
	
public: 
	PlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	void TurnCamY(float _playerY, bool isFlying, int ScreenHeight, int MapHeight);
	Mario* GetPlayer() { return player; }
	void AddObject(GameObject* obj);


	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

typedef PlayScene* LPPLAYSCENE;

