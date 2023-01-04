#pragma once

#include <d3dx10.h>
#include "Sprites.h"
#include "Textures.h"

#define TILE_WIDTH 16
#define TILE_HEIGHT 16
class Map
{
	int** Matrix;
	int TotalColsOfTitleSet, TotalRowsOfTileSet;
	int TotalColsOfMap, TotalRowsOfMap;
	int TotalTiles;
	LPTEXTURE TileSet;
	vector<LPSPRITE> Tiles;
public:
	Map(int idMap, int nTitleCols, int nTitleRows, int nMapCols, int nMapRows, int nTotalTiles);
	~Map();
	void CreateTilesFromTileSet();
	void LoadMatrix(LPCWSTR path);
	void Render();
	int GetTotalColsOfMap() { return this->TotalColsOfMap; }
	int GetTotalRowsOfMap() { return this->TotalRowsOfMap; }
};

