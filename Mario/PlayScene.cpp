#include <iostream>
#include <fstream>
#include "ThongSo.h"

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Coin.h"
#include "Platform.h"
#include "MovingPlatform.h"

#include "PlaySceneKeyHandler.h"
#include <d3dx9.h>

using namespace std;


PlayScene::PlayScene(int id, LPCWSTR filePath):
	Scene(id, filePath)
{
	//player = NULL;
	key_handler = new PlaySceneKeyHandler(this);
}


void PlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = Textures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	Sprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void PlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);

	
	LoadAssets(path.c_str());
}

void PlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new Animation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	Animations::GetInstance()->Add(ani_id, ani);
}

void PlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new AnimationSet();

	Animations* animations = Animations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	AnimationSets::GetInstance()->Add(ani_set_id, s);
}

void PlayScene::_ParseSection_MAPS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 7) return; // skip invalid lines

	int idMap = atoi(tokens[0].c_str());
	int tolRowTileSet = atoi(tokens[1].c_str());
	int tolColTileSet = atoi(tokens[2].c_str());
	int tolRowMap = atoi(tokens[3].c_str());
	int tolColMap = atoi(tokens[4].c_str());
	int totalTiles = atoi(tokens[5].c_str());
	wstring MatrixPath = ToWSTR(tokens[6]);

	this->map = new Map(idMap, tolRowTileSet, tolColTileSet, tolRowMap, tolColMap, totalTiles);
	map->LoadMatrix(MatrixPath.c_str());
	map->CreateTilesFromTileSet();
}

/*
	Parse a line in section [OBJECTS] 
*/
void PlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 3) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	AnimationSets* animation_sets = AnimationSets::GetInstance();

	GameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new Mario(x,y); 
		player = (Mario*)obj;  

		DebugOut(L"[INFO] Player object has been created!\n");
		break;
	case OBJECT_TYPE_GOOMBA: obj = new Goomba(x,y); break;
	case OBJECT_TYPE_BRICK: obj = new Brick(); break;
	case OBJECT_TYPE_KOOPAS:
	{
		int set_type = atoi(tokens[4].c_str());
		obj = new KoopaTroopa(x, y, set_type);
		break;
	}
	case OBJECT_TYPE_BLOCKS: obj = new Block(); break;
	case OBJECT_TYPE_GROUNDS: obj = new Ground(); break;
	case OBJECT_TYPE_PIPES:	obj = new Pipe(); break;
	case OBJECT_TYPE_INVISIBLEBRICK:
	{
		int set_type = atoi(tokens[4].c_str());
		obj = new InvisibleBrick(set_type);
		break;
	}
	case OBJECT_TYPE_PIRANHAPLANT:
	{
		int set_type = atoi(tokens[4].c_str());
		obj = new PiranhaPlant(x, y, set_type);
		break;
	}
	case OBJECT_TYPE_FIREPIRANHAPLANT:
	{
		int set_type = atoi(tokens[4].c_str());
		obj = new FirePiranhaPlant(x, y, set_type);
		break;
	}
	case OBJECT_TYPE_FIREPLANTBULLET:
	{

		obj = new FirePlantBullet();
		break;
	}
	case OBJECT_TYPE_COIN:
		obj = new Coin(); break;
	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
	if (object_type == OBJECT_TYPE_FIREPLANTBULLET)
		firebullet = ((FirePlantBullet*)obj);
}

void PlayScene::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = SCENE_SECTION_SPRITES; continue; };
		if (line == "[ANIMATIONS]") { section = SCENE_SECTION_ANIMATIONS; continue; };
		if (line == "[ANIMATION_SETS]") { section = SCENE_SECTION_ANIMATION_SETS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void PlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[MAP]") { section = SCENE_SECTION_MAPS; continue; };
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_MAPS: _ParseSection_MAPS(line); break;
			case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	Textures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void PlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<Enemy*>(objects[i]))
		{
			if (dynamic_cast<Enemy*>(objects[i])->IsAbleToActive() == true)
				objects[i]->Update(dt, &coObjects);
			else
				continue;
		}
		else
			objects[i]->Update(dt, &coObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return; 

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);

	Game *game = Game::GetInstance();
	int screenWidth = game->GetScreenWidth();
	int screenHeight = game->GetScreenHeight();
	int mapWidth = map->GetMapWidth();
	int mapHeight = map->GetMapHeiht();
	int oldCamY = game->GetCamY();

	TurnCamY(cy, player->IsFlying(), screenHeight, mapHeight);
	int Sx = 16, Sy = oldCamY;
	cx -= screenWidth / 2;
	cy -= screenHeight / 2;
	//if (player->x - screenWidth / 2 <= 16)
	//{
	//	cx = 16;
	//}
	//else if (player->x + screenWidth / 2 > mapWidth - 16)
	//	cx = (mapWidth - 16) - screenWidth;
	//if (player->y - screenHeight  < 16)
	//{
	//	cy = 16;
	//}
	//else
	//{
	//	if (_turnCamY)
	//		cy -= screenHeight / 2;
	//}
	// if (player->y + SCREEN_HEIGHT >= 448)
	//{
	//	if(_turnCamY && player->y + screenHeight /2 >= 448)
	//		cy = 448 - screenHeight;
	//	else if(!_turnCamY)
	//		cy = 448 - screenHeight;
	//}
	if (player->x < 16 + game->GetScreenWidth() / 2)
	{
		Sx = 16;
	}
	if (player->x > 16 + game->GetScreenWidth() / 2)
	{
		Sx = cx;
	}
	if (player->x + game->GetScreenWidth() / 2 > mapWidth - 16)
	{
		Sx = (mapWidth - 16) - screenWidth;
	}
	if (player->y + screenHeight > 448)
	{
		Sy = 448 - screenHeight;
	}
	if (player->y - screenHeight / 2 < 16)
		Sy = 16;
	else
	{
		if (_turnCamY)
			Sy = cy;
	}

	Game::GetInstance()->SetCamPos(round(Sx), round(Sy));
}

void PlayScene::Render()
{
	this->map->Render();
	for (int i = 1; i < objects.size(); i++)
		objects[i]->Render();
	objects[0]->Render();
}

/*
*	Clear all objects from this scene
*/
void PlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well 

*/
void PlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool PlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void PlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), PlayScene::IsGameObjectDeleted),
		objects.end());
}

void PlayScene::AddObject(GameObject* obj)
{
	this->objects.push_back(obj);
	DebugOut(L"Size: %d", this->objects.size());
}

void PlayScene::TurnCamY(float _playerY, bool isFlying, int ScreenHeight, int MapHeight)
{
	if (_turnCamY == true && _playerY > 448 - ScreenHeight / 2)
	{
		_turnCamY = false;
	}

	if (isFlying == true)
		_turnCamY = true;
}