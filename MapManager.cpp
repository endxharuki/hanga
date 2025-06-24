#include "MapManager.h"
#include "BlockMap.h"

MapManager* MapManager::Instance = MapManager::GetInstance();

void MapManager::InitManager()
{
	int a;
	currentMap = BlockMap::GetInstance()->GetCurrentMap();

	// �Q�[���I�u�W�F�i�[�z���������
	for (int y = 0; y < ONE_SCREEN_COLUMN; y++)
		for (int x = 0; x < ONE_SCREEN_RAW; x++)
		{
			delete map.objMap[y][x];
			map.objMap[y][x] = nullptr;
		}

}

void MapManager::SetBlock(int x, int y)
{
	D3DXVECTOR2 setPos = D3DXVECTOR2(x * CHIP_X_SIZE + CHIP_X_SIZE / 2, y * CHIP_Y_SIZE + CHIP_Y_SIZE / 2 + (SCREEN_HEIGHT - MAP_HEIGHT));
	D3DXVECTOR2 setSize = D3DXVECTOR2(CHIP_X_SIZE, CHIP_Y_SIZE);

	map.objMap[y][x] = new SetedBlock(setPos, setSize);
	map.objMap[y][x]->Start();
	int a = 0;
}

void MapManager::UpdateManager()
{
	// ������
	int chainNum = 0;
	bool isChain = false;
	std::vector<D3DXVECTOR2> chain;

	// ���l�񂻂�������m�F
	for (int yCnt = 0; yCnt < ONE_SCREEN_COLUMN; yCnt++)
	{
		for (int xCnt = 0; xCnt < ONE_SCREEN_RAW; xCnt++)
		{
			if (map.objMap[yCnt][xCnt] != nullptr)
			{
				chainNum++;
				isChain = true;
				chain.push_back(D3DXVECTOR2(xCnt, yCnt));
			}
			else if(chainNum >= 4 && isChain)
			{	// �l��ȏ㑵���Ă��Ă���ȏ㉡���Ȃ��ꍇ
				chainNum = 0;
				isChain = false;

				// �폜����
				WaitingChainBlock(chain);
				for (int i = 0; i < chain.size(); i++)
				{
					delete map.objMap[(int)chain[i].y][(int)chain[i].x];
					map.objMap[(int)chain[i].y][(int)chain[i].x] = nullptr; 
				}

				chain.clear();
			}
			else
			{
				chainNum = 0;
				isChain = false;
				chain.clear();
			}
		}

		// ���̍s�ɍs���ꍇ�͘A����؂�̂Ă�
		if (chainNum >= 4 && isChain)
		{
			chainNum = 0;
			isChain = false;

			// �폜����
			WaitingChainBlock(chain);
			for (int i = 0; i < chain.size(); i++)
			{
				delete map.objMap[(int)chain[i].y][(int)chain[i].x];
				map.objMap[(int)chain[i].y][(int)chain[i].x] = nullptr;
			}

			chain.clear();
		}
		else
		{
			chainNum = 0;
			isChain = false;
			chain.clear();
		}
	}
}

void MapManager::WaitingChainBlock(std::vector<D3DXVECTOR2> obj)
{
	const int waitingNum = 3;
	int currentFlashingNum = 0;
	DWORD currentTime = timeGetTime();
	DWORD prevTime = currentTime;

	// �҂�����
	while (1)
	{
		DWORD currentTime = timeGetTime();

		if (currentTime >= prevTime + (30 * waitingNum))
		{
			return;
		}
	}
}

void MapManager::DrawSetBlock()
{
	for (int y = 0; y < ONE_SCREEN_COLUMN; y++)
		for (int x = 0; x < ONE_SCREEN_RAW; x++)
			if (map.objMap[y][x] != nullptr) map.objMap[y][x]->Draw();
}

void MapManager::UnInitSetBlock()
{
	for (int y = 0; y < ONE_SCREEN_COLUMN; y++)
	{
		for (int x = 0; x < ONE_SCREEN_RAW; x++)
		{
			delete map.objMap[y][x];
			map.objMap[y][x] = nullptr;
		}
	}
}