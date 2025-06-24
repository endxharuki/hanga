#pragma once

#ifndef _MAP_MANAGER_H_
#define _MAP_MANAGER_H_

#include "BlockMap.h"
#include "SetedBlock.h"

typedef struct {
	SetedBlock* objMap[ONE_SCREEN_COLUMN][ONE_SCREEN_RAW];
} CUR_OBJ_MAP;

class MapManager
{
public:
	static MapManager* GetInstance()
	{
		if (Instance == nullptr)
			Instance = new MapManager;
		return Instance;
	}

	void InitManager();

	CUR_OBJ_MAP* GetObj() { return &map; }

	void SetBlock(int x, int y);

	void UpdateManager();

	void DrawSetBlock();

	void UnInitSetBlock();

private:
	static MapManager* Instance;
	MapManager() {}

	void WaitingChainBlock(std::vector<D3DXVECTOR2> obj);

	CUR_MAP* currentMap;
	CUR_OBJ_MAP map;
};

#endif // !_MAP_MANAGER_H_
