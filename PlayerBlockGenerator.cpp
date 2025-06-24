// ========================================
// プレイヤー設置ブロック生成クラス [PlayerBlockGenerator.cpp]
// Author: 西窪 統吉桜	Date: 2023/09/08
// ========================================
#include "PlayerBlockGenerator.h"
#include "BlockMap.h"
#include "sprite.h"
#include "OnGameData.h"

PlayerBlockGenerator* PlayerBlockGenerator::Instance = PlayerBlockGenerator::GetInstance();

// 生成されるブロックの種類
int generateBlock[5][5][3][3] = {

	{	// s1
		{
			{0, 0, 0},
			{0, 1, 1},
			{0, 0, 0}
		},

		{
			{0, 0, 0},
			{0, 1, 1},
			{0, 0, 1}
		},

		{
			{0, 0, 0},
			{0, 1, 1},
			{0, 1, 1}
		},

		{
			{0, 0, 0},
			{0, 0, 1},
			{0, 1, 1}
		},

		{
			{0, 0, 0},
			{0, 1, 0},
			{0, 0, 0}
		},
	},

	{	// s2
		{
			{0, 0, 1},
			{0, 0, 1},
			{0, 0, 1}
		},

		{
			{0, 0, 0},
			{0, 1, 0},
			{1, 1, 1}
		},

		{
			{0, 1, 1},
			{0, 0, 1},
			{0, 1, 1}
		},

		{
			{1, 1, 0},
			{1, 0, 0},
			{0, 0, 0}
		},

		{
			{0, 1, 0},
			{0, 1, 0},
			{0, 0, 0}
		},
	},

	{	// s3
		{
			{0, 0, 1},
			{0, 1, 0},
			{0, 0, 1}
		},

		{
			{1, 1, 0},
			{0, 1, 0},
			{0, 0, 0}
		},

		{
			{0, 0, 0},
			{1, 1, 1},
			{0, 1, 0}
		},

		{
			{0, 0, 0},
			{0, 0, 0},
			{1, 1, 1}
		},

		{
			{0, 0, 0},
			{0, 1, 1},
			{0, 0, 1}
		},
	},

	{	// s4
		{
			{0, 0, 1},
			{0, 0, 1},
			{0, 0, 1}
		},

		{
			{0, 0, 0},
			{0, 1, 0},
			{1, 1, 1}
		},

		{
			{0, 0, 0},
			{1, 0, 1},
			{1, 1, 1}
		},

		{
			{1, 1, 0},
			{1, 0, 0},
			{0, 0, 0}
		},

		{
			{0, 0, 0},
			{0, 0, 0},
			{0, 0, 0}
		},
	},

	{	// s5
		{
			{0, 0, 1},
			{0, 0, 1},
			{0, 0, 1}
		},

		{
			{0, 0, 0},
			{0, 1, 0},
			{1, 1, 1}
		},

		{
			{0, 0, 0},
			{1, 0, 1},
			{1, 1, 1}
		},

		{
			{1, 1, 0},
			{1, 0, 0},
			{0, 0, 0}
		},

		{
			{0, 0, 0},
			{0, 0, 0},
			{0, 0, 0}
		},
	},

};

void PlayerBlockGenerator::InitGenerator()
{
	nextShape = rand() % 5;
	GenerateBlock();
}

PlayerBlockGenerator* PlayerBlockGenerator::GetInstance()
{
	if (Instance == nullptr)
		Instance = new PlayerBlockGenerator();

	return Instance;
}

void PlayerBlockGenerator::GenerateBlock()
{
	const D3DXVECTOR2 setPos = D3DXVECTOR2(PLAYER_BLOCK_START_X, PLAYER_BLOCK_START_Y);

	// ============
	// 生成処理
	// ============
	



	// 万が一ブロックがある場合消去する
	if (pBlock != nullptr)
		delete pBlock;

	pBlock = new PlayerBlock(setPos, generateBlock[OnGameData::GetInstance()->GetStageNum()][nextShape]);
	pBlock->Start();

	// ============
	// 後処理
	// ============

	// 次のブロックを確定させる
	nextShape = rand() % 5;


}

void PlayerBlockGenerator::CheckBlock()
{
	if (pBlock->GetDeleteRequest())
	{
		delete pBlock;
		pBlock = nullptr;

		GenerateBlock();
	}
}

void PlayerBlockGenerator::DrawNextShape()
{
	const int centerX = 1;
	const int centerY = 1;

	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			if (generateBlock[OnGameData::GetInstance()->GetStageNum()][nextShape][y][x] == 0) continue;
			D3DXVECTOR2 pos;
			pos.x = (x - centerX) * CHIP_X_SIZE + PLAYER_BLOCK_START_X;
			pos.y = (y - centerY) * CHIP_Y_SIZE + PLAYER_BLOCK_START_Y;

			DrawSpriteColor(NULL,
				pos.x, pos.y,
				CHIP_X_SIZE, CHIP_Y_SIZE,
				0.0f, 0.0f,
				1.0f, 1.0f,
				0.0f, 0.0f, 1.0f, 0.5f);
		}
	}
}

