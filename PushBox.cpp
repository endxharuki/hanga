#include "PushBox.h"
#include "BlockMap.h"
#include "ColiderPool.h"
#include "OnGameData.h"

D3DXVECTOR2 PushBox::Push(D3DXVECTOR2 move) {
	// �ŉ��ԂłȂ��Ȃ珈�����Ȃ�
	if (!OnGameData::GetInstance()->HasSwap()) return D3DXVECTOR2(0.0f, 0.0f);

	transform = obj->GetTransform();
	
	D3DXVECTOR2 pos = transform->GetPos();

	transform->SetPos(pos.x + move.x, pos.y + move.y);
	BlockMap::GetInstance()->ColidAllBlock(transform, obj->GetComponent<ColiderPool>()->GetColider()[0]);

	// �Փ˔����̈ʒu
	D3DXVECTOR2 friction = transform->GetPos();

	return friction - pos;
}