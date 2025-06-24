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
	void DrawMap(int layer);		// ���C���[�ꖇ��
	void DrawLayer2();

	// �Ƃɂ����S�Ẵu���b�N�Ƃ̏Փ˔�����s��
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

	// x���Ƀ\�[�g���ꂽlist����߂��̃}�b�v���擾
	// �������Ă��邩�ǂ����͂܂����肵�Ȃ�
	// �v�Z�ʂ����炷���߂̊֐�
	std::list<GameObject*> GetSortedMap(const D3DXVECTOR2& pos, const D3DXVECTOR2& size);

private:
	BlockMap() {};
	static BlockMap* Instance;

	D3DXVECTOR2 CalcBlockSize(D3DXVECTOR2 startPos, D3DXVECTOR2 endPos);
	bool IsCheckedChip(D3DXVECTOR2 current, std::list<D3DXVECTOR2> checkMap);

	std::list<GameObject*> map[MAX_LAYER];
	std::list<GameObject*> upMap;	// ���� x��
	std::list<GameObject*> downMap;	// �~�� x�E
	std::list<GameObject*> map2;
	std::list<GameObject*> layer2Map;
};


