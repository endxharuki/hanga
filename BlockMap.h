#pragma once

#include "GameObject.h"
#include <List>
#include "Goal.h"

#define MAP_HEIGHT (SCREEN_HEIGHT)

#define ONE_SCREEN_RAW (20)		// 一画面中に描画される横のマス数
#define ONE_SCREEN_COLUMN (20)	// 一画面中に描画される縦のマス数
#define CHIP_X_SIZE (SCREEN_WIDTH / ONE_SCREEN_RAW)
#define CHIP_Y_SIZE (SCREEN_HEIGHT / ONE_SCREEN_COLUMN)
#define MAX_MAP_WIDE (150)

class BlockMap
{
public:
	static BlockMap* GetInstance() { return Instance; }

	void InitMap(int stageNum);
	void StartMap();
	void UnInitMap();
	void UpdateMap();
	void UpdateMap2();
	void DrawMap();		// レイヤー一枚目
	void DrawMap2();	// レイヤー二枚目
	void DrawLayer2();

	// とにかく全てのブロックとの衝突判定を行う
	void ColidAllBlock(
		Transform2D* setTransform,
		Colider2D setColid
		);

	bool IsColidAllBlock(
		Transform2D* setTransform
		, Colider2D setColid);

	std::list<GameObject*> GetMap() { return map; }
	std::list<GameObject*> GetMap2() { return map2; }
	std::list<GameObject*> GetLayer2() { return layer2Map; }

private:
	BlockMap() {};
	static BlockMap* Instance;

	D3DXVECTOR2 CalcBlockSize(D3DXVECTOR2 startPos, D3DXVECTOR2 endPos);
	bool IsCheckedChip(D3DXVECTOR2 current, std::list<D3DXVECTOR2> checkMap);

	std::list<GameObject*> map;
	std::list<GameObject*> map2;
	std::list<GameObject*> layer2Map;
};


