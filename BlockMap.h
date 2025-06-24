#pragma once

#include "GameObject.h"
#include <List>
#include "Goal.h"

#define MAP_HEIGHT (SCREEN_HEIGHT)

#define CHIP_X_SIZE (80.0f)
#define CHIP_Y_SIZE (80.0f)
#define MAX_LAYER (3)

class BlockMap
{
public:
	static BlockMap* GetInstance() { return Instance; }

	void InitMap(int stageNum);
	void StartMap();
	void UnInitMap();
	void UpdateMap();
	void DrawMap(int layer);		// レイヤー一枚目
	void DrawLayer2();

	// とにかく全てのブロックとの衝突判定を行う
	void ColidAllBlock(
		Transform2D* setTransform,
		Colider2D setColid
		);

	bool IsColidAllBlock(
		Transform2D* setTransform
		, Colider2D setColid);

	std::list<GameObject*> GetMap(int layer) { 
		if (layer > MAX_LAYER)
		{
			std::list<GameObject*> empty;
			return empty;
		}
		return map[layer]; 
	}
	std::list<GameObject*> GetMap2() { return map2; }
	std::list<GameObject*> GetLayer2() { return layer2Map; }

	// x軸にソートされたlistから近くのマップを取得
	// 当たっているかどうかはまだ判定しない
	// 計算量を減らすための関数
	std::list<GameObject*> GetSortedMap(const D3DXVECTOR2& pos, const D3DXVECTOR2& size);

private:
	BlockMap() {};
	static BlockMap* Instance;

	D3DXVECTOR2 CalcBlockSize(D3DXVECTOR2 startPos, D3DXVECTOR2 endPos);
	bool IsCheckedChip(D3DXVECTOR2 current, std::list<D3DXVECTOR2> checkMap);

	std::list<GameObject*> map[MAX_LAYER];
	std::list<GameObject*> upMap;	// 昇順 x左
	std::list<GameObject*> downMap;	// 降順 x右
	std::list<GameObject*> map2;
	std::list<GameObject*> layer2Map;
};


