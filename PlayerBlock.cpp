// ========================================
// プレイヤー設置ブロック生成クラス [PlayerBlock.cpp]
// Author: 西窪 統吉桜	Date: 2023/09/08
// ========================================
#include "PlayerBlock.h"
#include "BlockMap.h"

void PlayerBlock::CreateColid()
{
	// 形を決める中心点
	const int centerX = 1;
	const int centerY = 1;

	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			if (blockShape[y][x] == 1)
			{
				// オフセットの計算
				D3DXVECTOR2 offset;
				offset.x = (x - centerX) * CHIP_X_SIZE;
				offset.y = (y - centerY) * CHIP_Y_SIZE;

				colider.push_back( Colider2D(&transform, offset, D3DXVECTOR2(CHIP_X_SIZE, CHIP_Y_SIZE), PLAYER_BLOCK_TAG) );
			}
		}
	}
}

