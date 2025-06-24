#include "BlockMap.h"
#include "sprite.h"
#include "MainInGame.h"
#include "MainTitle.h"
#include "BeanDemon.h"
#include "MapLoader.h"
#include "PermationBlock.h"
#include "MoveSwitchBlock.h"
#include "Switch.h"
#include "Board.h"
#include "CheckPoint.h"

BlockMap* BlockMap::Instance = new BlockMap();



void BlockMap::InitMap(int stageNum)
{
	// 初期化処理
	for (int i = 0; i < MAX_LAYER; i++)map[i].clear();

	if (stageNum == 0)
	{
		LoadMap((char*)"data/Stage/new_title.dat", map, &MainTitle::objectPool);
	}
	else if (stageNum == 1)
	{
		LoadMap((char*)"data/Stage/1-1.dat", map, &MainInGame::objectPool);

		//MainInGame::checkPoint = new CheckPoint(D3DXVECTOR2(1110.0f, 1140.0f), D3DXVECTOR2(CHIP_X_SIZE, CHIP_Y_SIZE * 2));
		//MainInGame::objectPool.Add(MainInGame::checkPoint);

		MainInGame::objectPool.Add(new Board(D3DXVECTOR2(610.0f, 1240.0f), D3DXVECTOR2(80.0f, 80.0f), LoadTexture((char*)"data/TEXTURE/board_tutorial5.png")));

		MainInGame::objectPool.Add(new Board(D3DXVECTOR2(1170.0f, 1240.0f), D3DXVECTOR2(80.0f, 80.0f), LoadTexture((char*)"data/TEXTURE/board_tutorial1.png")));

		MainInGame::objectPool.Add(new Board(D3DXVECTOR2(4090.0f, 1160.0f), D3DXVECTOR2(80.0f, 80.0f), LoadTexture((char*)"data/TEXTURE/board_tutorial6.png")));

		MainInGame::objectPool.Add(new Board(D3DXVECTOR2(6200.0f, 1160.0f), D3DXVECTOR2(80.0f, 80.0f), LoadTexture((char*)"data/TEXTURE/board_tutorial2.png")));

		MainInGame::objectPool.Add(new Board(D3DXVECTOR2(11730.0f, 920.0f), D3DXVECTOR2(80.0f, 80.0f), LoadTexture((char*)"data/TEXTURE/board_tutorial3.png")));

		MainInGame::objectPool.Add(new Board(D3DXVECTOR2(15090.0f, 1080.0f), D3DXVECTOR2(80.0f, 80.0f), LoadTexture((char*)"data/TEXTURE/board_tutorial4.png")));
	}
	else if (stageNum == 2)
	{
		LoadMap((char*)"data/Stage/1-2.dat", map, &MainInGame::objectPool);
	}
	else if (stageNum == 3)
	{
		LoadMap((char*)"data/Stage/1-3.dat", map, &MainInGame::objectPool);
	}
	else if (stageNum == 4)
	{
		LoadMap((char*)"data/Stage/AllEXstage1.dat", map, &MainInGame::objectPool);
	}

	// xの昇順に並べる
	// HACK: エディタでやってもいいかも
	map[0].sort([](GameObject* p1, GameObject* p2) {
		D3DXVECTOR2 pos1, pos2, size1, size2;
		pos1 = p1->GetTransform()->GetPos();
		pos2 = p2->GetTransform()->GetPos();
		size1 = p1->GetTransform()->GetSize();
		size2 = p2->GetTransform()->GetSize();

		return pos1.x - size1.x * 0.5f < pos2.x - size2.x * 0.5f;
		});

	upMap = map[0];

	// xの降順に並べる
	map[0].sort([](GameObject* p1, GameObject* p2) {
		D3DXVECTOR2 pos1, pos2, size1, size2;
		pos1 = p1->GetTransform()->GetPos();
		pos2 = p2->GetTransform()->GetPos();
		size1 = p1->GetTransform()->GetSize();
		size2 = p2->GetTransform()->GetSize();

		return pos1.x + size1.x * 0.5f > pos2.x + size2.x * 0.5f;
		});

	downMap = map[0];

}


void BlockMap::StartMap()
{
	for (int i = 0; i < MAX_LAYER; i++)
	{
		for (GameObject* obj : map[i]) obj->Start();
	}

	for (GameObject* object : layer2Map)
		object->Start();
}

void BlockMap::UnInitMap()
{
	for (int i = 0; i < MAX_LAYER; i++)
	{
		for (GameObject* obj : map[i]) delete obj;
		map[i].clear();
	}

	for (GameObject* object : layer2Map) delete object;
	layer2Map.clear();
}

void BlockMap::UpdateMap()
{
	for (GameObject* obj : map[0]) obj->Update();
}


void BlockMap::DrawMap(int layer)
{
	for (GameObject* obj : map[layer])
	{
		obj->Draw();
	}
}

bool BlockMap::IsColidAllBlock(Transform2D* setTransform, Colider2D setColid)
{
	D3DXVECTOR2 pos = setColid.GetPos();
	D3DXVECTOR2 prevPos = setColid.GetPrevPos();
	D3DXVECTOR2 size = setColid.GetSize();

	D3DXVECTOR2 fricBlockPos;
	D3DXVECTOR2 fricBlockSize;

	bool isFriction = false;

	for (GameObject* object : map[0])
	{
		Colider2D colid = object->GetComponent<ColiderPool>()->GetColider()[0];
		D3DXVECTOR2 blockPos = colid.GetPos();
		D3DXVECTOR2 blockSize = colid.GetSize();

		D3DXVECTOR2 distance = D3DXVECTOR2(fabsf(pos.x - blockPos.x), fabsf(pos.y - blockPos.y));
		D3DXVECTOR2 sumSize = D3DXVECTOR2((size.x + blockSize.x) / 2.0f, (size.y + blockSize.y) / 2.0f);

		if (distance.x < sumSize.x &&
			distance.y < sumSize.y)
		{	// 衝突時
			return true;
		}
	} // 衝突判定終了

	return false;
}


void BlockMap::ColidAllBlock(Transform2D* setTransform, Colider2D setColid)
{
	D3DXVECTOR2 pos = setColid.GetPos();
	D3DXVECTOR2 prevPos = setColid.GetPrevPos();
	D3DXVECTOR2 size = setColid.GetSize();

	D3DXVECTOR2 fricBlockPos;
	D3DXVECTOR2 fricBlockSize;
	D3DXVECTOR2 fricSetPos;

	bool isFriction = false;
	D3DXVECTOR2 moveAll = { 0.0f, 0.0f };

	std::list<GameObject*> checkMap;
	checkMap = map[0];

	for (GameObject* object : checkMap)
	{
		Colider2D colid = object->GetComponent<ColiderPool>()->GetColider()[0];
		D3DXVECTOR2 blockPos = colid.GetPos();
		D3DXVECTOR2 blockSize = colid.GetSize();

		D3DXVECTOR2 setPos = pos + moveAll;

		D3DXVECTOR2 distance = D3DXVECTOR2(fabsf(setPos.x - blockPos.x), fabsf(setPos.y - blockPos.y));
		D3DXVECTOR2 sumSize = D3DXVECTOR2((size.x + blockSize.x) / 2.0f, (size.y + blockSize.y) / 2.0f);

		if (distance.x < sumSize.x &&
			distance.y < sumSize.y)
		{	// 衝突時
			fricBlockPos = blockPos;
			fricBlockSize = blockSize;
			isFriction = true;
			

			// ==============
			// 衝突辺の確認
			// ==============
			float setTop = setPos.y - size.y / 2;
			float setBottom = setPos.y + size.y / 2;
			float setLeft = setPos.x - size.x / 2;
			float setRight = setPos.x + size.x / 2;

			float setPrevTop = prevPos.y - size.y / 2;
			float setPrevBottom = prevPos.y + size.y / 2;
			float setPrevLeft = prevPos.x - size.x / 2;
			float setPrevRight = prevPos.x + size.x / 2;

			float blockTop = fricBlockPos.y - fricBlockSize.y / 2;
			float blockBottom = fricBlockPos.y + fricBlockSize.y / 2;
			float blockLeft = fricBlockPos.x - fricBlockSize.x / 2;
			float blockRight = fricBlockPos.x + fricBlockSize.x / 2;

			// 辺の判定
			if (setPrevRight <= blockLeft && setRight >= blockLeft && setLeft <= blockLeft)
			{	// ブロックの左に衝突
				if (fabsf(setPrevBottom - blockTop) > 0.01f)moveAll.x += blockLeft - setRight;
			}
			else if (setPrevLeft >= blockRight && setLeft <= blockRight && setRight >= blockRight)
			{	// ブロックの右に衝突
				if (fabsf(setPrevBottom - blockTop) > 0.01f)moveAll.x += blockRight - setLeft;
			}
			else if (setPrevBottom <= blockTop && setTop <= blockTop && setBottom >= blockTop)
			{	// ブロックの上に衝突
				moveAll.y += blockTop - setBottom;
			}
			else if (setPrevTop >= blockBottom && setTop <= blockBottom && setBottom >= blockBottom)
			{	// ブロックの下に衝突
				moveAll.y += blockBottom - setTop;
			}
		}
	} // 衝突判定終了

	D3DXVECTOR2 currentPos = setTransform->GetPos();
	setTransform->SetPos(currentPos.x + moveAll.x, currentPos.y + moveAll.y);
}

std::list<GameObject*> BlockMap::GetSortedMap(const D3DXVECTOR2& pos, const D3DXVECTOR2& size)
{
	std::list<GameObject*> upList;
	std::list<GameObject*> outList;

	const float left  = pos.x - size.x * 0.5f;
	const float right = pos.x + size.x * 0.5f;

	for (GameObject* p : upMap)
	{
		D3DXVECTOR2 targetPos = p->GetTransform()->GetPos();
		D3DXVECTOR2 targetSize = p->GetTransform()->GetSize();

		if (right >= targetPos.x - targetSize.x * 0.5f)
		{	// ターゲットの左よりも設定した右のほうが出ていれば
			upList.push_back(p);
		}
		else
		{
			break;// 当たる可能性のものはないので抜ける
		}
	}

	for (GameObject* p : downMap)
	{
		D3DXVECTOR2 targetPos = p->GetTransform()->GetPos();
		D3DXVECTOR2 targetSize = p->GetTransform()->GetSize();

		if (left <= targetPos.x + targetSize.x * 0.5f)
		{	// ターゲットの右よりも設定した左のほうが出ていれば
			auto it = std::find(upList.begin(), upList.end(), p);

			if (it != upList.end())
			{
				outList.push_back(p);
			}
		}
		else
		{
			break;// 当たる可能性のものはないので抜ける
		}
	}

	return outList;
}

D3DXVECTOR2 BlockMap::CalcBlockSize(D3DXVECTOR2 startPos, D3DXVECTOR2 endPos)
{
	D3DXVECTOR2 calcPos = D3DXVECTOR2(endPos.x - startPos.x, endPos.y - startPos.y);
	if (calcPos.x == 0 && calcPos.y == 0) return D3DXVECTOR2(CHIP_X_SIZE, CHIP_Y_SIZE);

	return D3DXVECTOR2(CHIP_X_SIZE * (calcPos.x + 1), CHIP_Y_SIZE * (calcPos.y + 1));
}

// 確認済みブロックであるか判断する
bool BlockMap::IsCheckedChip(D3DXVECTOR2 current, std::list<D3DXVECTOR2> checkMap)
{
	for (D3DXVECTOR2 map : checkMap)
	{
		// 探索済みブロックだった場合
		if (current.x == map.x && current.y == map.y)
		{
			return true;
		}
	}

	return false;
}
