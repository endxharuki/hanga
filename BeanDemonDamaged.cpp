#include "BeanDemonDamaged.h"
#include "BeanDemon.h"
#include "Camera.h"
#include "sprite.h"

void BeanDemonDamaged::SetUp()
{
	CleanRequest();

	isBlink = false;
	frameCnt = 0;
}

void BeanDemonDamaged::CleanUp()
{
}

void BeanDemonDamaged::OnUpdate()
{
	++frameCnt;
	if (frameCnt % blinkFrame == 0)
	{
		isBlink = !isBlink;
	}

	if (frameCnt >= blinkFrame * maxBlinkCnt)
	{
		isBlink = false; // ‹­§“I‚ÉŒ©‚¦‚é‚æ‚¤‚É‚·‚é
		ChangeRequest("Move");
	}
}

void BeanDemonDamaged::OnDraw()
{
	if (!isBlink) return;

	D3DXVECTOR2 pos = obj->GetTransform()->GetPos();
	D3DXVECTOR2 size = obj->GetTransform()->GetSize();
	D3DXVECTOR2 cameraPos = Camera::GetInstance()->GetOriginPos();

	D3DXVECTOR2 drawPos = pos - cameraPos;

	DrawSpriteColor(obj->idleTex,
		drawPos.x, drawPos.y,
		size.x, size.y,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	);
}
