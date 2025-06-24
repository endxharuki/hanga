#include "BeanDemonStan.h"
#include "BeanDemon.h"
#include "Camera.h"
#include "sprite.h"

void BeanDemonStan::SetUp()
{
	CleanRequest();

	frameCnt = 0;
}

void BeanDemonStan::CleanUp()
{

}

void BeanDemonStan::OnUpdate()
{
	++frameCnt;
	if (frameCnt >= stanFrame)
	{
		obj->ReturnedStan();
		ChangeRequest("Move");
		return;
	}
}

void BeanDemonStan::OnDraw()
{
	D3DXVECTOR2 pos = obj->GetTransform()->GetPos();
	D3DXVECTOR2 size = obj->GetTransform()->GetSize();
	D3DXVECTOR2 cameraPos = Camera::GetInstance()->GetOriginPos();

	D3DXVECTOR2 drawPos = pos - cameraPos;

	DrawSpriteColor(obj->stanTex,
		drawPos.x, drawPos.y,
		size.x, size.y,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	);
}
