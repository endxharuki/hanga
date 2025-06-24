#include "PushWoodBox.h"
#include "BlockMap.h"
#include "ColiderPool.h"
#include "OnGameData.h"

D3DXVECTOR2 PushWoodBox::Push(D3DXVECTOR2 move)
{
	transform = obj->GetTransform();

	D3DXVECTOR2 pos = transform->GetPos();

	transform->SetPos(pos.x + move.x, pos.y + move.y);
	BlockMap::GetInstance()->ColidAllBlock(transform, obj->GetComponent<ColiderPool>()->GetColider()[0]);

	// Õ“Ë”»’èŒã‚ÌˆÊ’u
	D3DXVECTOR2 friction = transform->GetPos();

	return friction - pos;
}