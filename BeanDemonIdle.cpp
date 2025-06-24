#include "BeanDemonIdle.h"
#include "BeanDemon.h"
#include "sprite.h"
#include "Camera.h"

void BeanDemonIdle::SetUp()
{
	CleanRequest();
}

void BeanDemonIdle::CleanUp()
{
}

void BeanDemonIdle::OnUpdate()
{
	if (obj->GetActive()) ChangeRequest("Move");

	// TODO: 設置個所が不自然な場所になるなら重力に従ってブロックと当たり判定を持たせて
	//		 埋まりを解消すべきか
}

void BeanDemonIdle::OnDraw()
{
	D3DXVECTOR2 pos = obj->GetTransform()->GetPos();
	D3DXVECTOR2 size = obj->GetTransform()->GetSize();
	D3DXVECTOR2 cameraPos = Camera::GetInstance()->GetOriginPos();

	D3DXVECTOR2 drawPos = pos - cameraPos;

	DrawSpriteColor( obj->idleTex,
		drawPos.x, drawPos.y,
		size.x, size.y,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	);
}
